# LlamaIndex Agents Workflow 架构分析

## 1. 整体架构

```
┌─────────────────────────────────────────────────────────────┐
│                     AgentWorkflow                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐          │
│  │FunctionAgent│  │ ReActAgent  │  │ CodeActAgent│  ...     │
│  └─────────────┘  └─────────────┘  └─────────────┘          │
│         ↑               ↑               ↑                    │
│         └───────────────┴───────────────┘                    │
│                         │                                    │
│              BaseWorkflowAgent (抽象基类)                    │
│                         │                                    │
│              ┌──────────┴──────────┐                        │
│              │  Handoff Tool (意图路由)                      │
│              └─────────────────────┘                        │
│                         │                                    │
│         ┌───────────────┼───────────────┐                   │
│         ▼               ▼               ▼                    │
│   SharedMemory     SharedState     ToolResults              │
│   (ChatMemoryBuffer) (Dict)      (聚合)                     │
└─────────────────────────────────────────────────────────────┘
```

### 核心文件结构

| 文件 | 行数 | 用途 |
|------|------|------|
| `base_agent.py` | 645 | 抽象基类，定义6个工作流步骤 |
| `multi_agent_workflow.py` | 720 | 多代理工作流管理器 |
| `function_agent.py` | 195 | 函数调用代理 |
| `react_agent.py` | 300 | ReAct推理代理 |
| `codeact_agent.py` | 399 | 代码执行代理 |
| `workflow_events.py` | 144 | 事件定义系统 |
| `prompts.py` | 15 | 提示词模板 |

### 类继承关系

```
Workflow (基础类)
├── BaseWorkflowAgent (抽象基类)
│   ├── FunctionAgent (使用LLM函数调用)
│   ├── ReActAgent (思维链推理)
│   └── CodeActAgent (代码执行)
└── AgentWorkflow (多代理管理器)
```

---

## 2. 多意图识别机制

LlamaIndex **不是传统的意图分类模型**，而是通过 **Handoff Tool + LLM 决策** 实现多意图路由。

### 2.1 Handoff 函数

**位置**: `multi_agent_workflow.py:57-76`

```python
async def handoff(ctx: Context, to_agent: str, reason: str) -> str:
    """Handoff control of that chat to the given agent."""
    agents: list[str] = await ctx.store.get("agents")
    current_agent_name: str = await ctx.store.get("current_agent_name")

    # 验证目标代理是否存在
    if to_agent not in agents:
        return f"Agent {to_agent} not found..."

    # 检查当前代理是否有权限handoff到目标代理
    can_handoff_to: dict[str, list[str]] = await ctx.store.get("can_handoff_to")
    if to_agent not in can_handoff_to.get(current_agent_name, []):
        return f"Agent {to_agent} cannot hand off..."

    # 设置下一个代理
    await ctx.store.set("next_agent", to_agent)
    return f"Agent {to_agent} is now handling the request..."
```

### 2.2 Handoff Tool 动态生成

**位置**: `multi_agent_workflow.py:198-228`

```python
def _get_handoff_tool(self, current_agent: BaseWorkflowAgent) -> Optional[AsyncBaseTool]:
    # 单代理时不需要handoff
    if len(self.agents) == 1:
        return None

    # 收集所有可handoff的代理描述
    agent_info = {cfg.name: cfg.description for cfg in self.agents.values()}

    # 过滤：移除自己和不允许handoff的代理
    configs_to_remove = []
    for name in agent_info:
        if name == current_agent.name:
            configs_to_remove.append(name)
        elif (current_agent.can_handoff_to is not None
              and name not in current_agent.can_handoff_to):
            configs_to_remove.append(name)

    for name in configs_to_remove:
        agent_info.pop(name)

    # 生成带有代理描述的handoff提示
    fn_tool_prompt = self.handoff_prompt.format(agent_info=str(agent_info))
    return FunctionTool.from_defaults(
        async_fn=handoff,
        description=fn_tool_prompt,
        return_direct=True
    )
```

### 2.3 Handoff Prompt 模板

**位置**: `prompts.py:1-6`

```python
DEFAULT_HANDOFF_PROMPT = """Useful for handing off to another agent.
If you are currently not equipped to handle the user's request,
or another agent is better suited to handle the request,
please hand off to the appropriate agent.

Currently available agents:
{agent_info}
"""

DEFAULT_HANDOFF_OUTPUT_PROMPT = "Agent {to_agent} is now handling the request due to the following reason: {reason}.\nPlease continue with the current request."
```

### 2.4 工作原理

1. 每个代理启动时，会自动获得一个 `handoff` 工具
2. 工具描述中包含了所有可切换代理的 **名称+描述**
3. LLM 根据用户请求，判断当前代理是否合适
4. 如果不合适，LLM 主动调用 handoff 工具切换到更合适的代理
5. 这是一种 **隐式多意图识别**：LLM 自己理解意图并做出路由决策

---

## 3. 工作流步骤

### 3.1 六步工作流

**位置**: `base_agent.py`

```
StartEvent
    ↓
init_run (初始化 memory、state)
    ↓
setup_agent (准备 LLM 输入，注入 state)
    ↓
run_agent_step (执行 take_step)
    ↓
parse_agent_output ──→ 有 tool_calls? ──→ Yes ──→ call_tool (并行执行)
    ↓ No                                            ↓
StopEvent                              aggregate_tool_results
                                               ↓
                                       检查 next_agent?
                                               ↓ Yes
                                       切换到新代理 → AgentInput
```

### 3.2 步骤详解

#### init_run - 初始化

**位置**: `base_agent.py:309-361`

```python
@step
async def init_run(self, ctx: Context, ev: AgentWorkflowStartEvent) -> AgentInput:
    """Sets up the workflow and validates inputs."""
    await self._init_context(ctx, ev)

    # 处理用户消息
    user_msg = ev.get("user_msg")
    if isinstance(user_msg, str):
        user_msg = ChatMessage(role="user", content=user_msg)

    # 添加到 memory
    memory: BaseMemory = await ctx.store.get("memory")
    if user_msg:
        await memory.aput(user_msg)

    input_messages = await memory.aget()
    return AgentInput(input=input_messages, current_agent_name=self.name)
```

#### setup_agent - 准备输入

**位置**: `base_agent.py:363-389`

```python
@step
async def setup_agent(self, ctx: Context, ev: AgentInput) -> AgentSetup:
    """Main agent handling logic."""
    llm_input = [*ev.input]

    # 添加 system prompt
    if self.system_prompt:
        llm_input = [
            ChatMessage(role="system", content=self.system_prompt),
            *llm_input,
        ]

    # 注入当前状态到最后一条消息
    state = await ctx.store.get("state", default=None)
    if state and not formatted_input_with_state:
        for block in llm_input[-1].blocks[::-1]:
            if isinstance(block, TextBlock):
                block.text = self.state_prompt.format(state=state, msg=block.text)
                break

    return AgentSetup(input=llm_input, current_agent_name=ev.current_agent_name)
```

#### run_agent_step - 执行代理

**位置**: `base_agent.py:391-406`

```python
@step
async def run_agent_step(self, ctx: Context, ev: AgentSetup) -> AgentOutput:
    """Run the agent."""
    memory = await ctx.store.get("memory")
    tools = await self.get_tools(user_msg_str or "")

    # 调用子类实现的 take_step
    agent_output = await self.take_step(ctx, ev.input, tools, memory)

    ctx.write_event_to_stream(agent_output)
    return agent_output
```

#### parse_agent_output - 解析输出

**位置**: `base_agent.py:408-501`

```python
@step
async def parse_agent_output(self, ctx: Context, ev: AgentOutput):
    # 检查迭代次数限制
    if num_iterations >= max_iterations:
        raise WorkflowRuntimeError("Max iterations reached!")

    # 如果有重试消息，重新执行
    if ev.retry_messages:
        return AgentInput(input=[*history, *ev.retry_messages], ...)

    # 如果没有工具调用，生成最终输出
    if not ev.tool_calls:
        output = await self.finalize(ctx, ev, memory)

        # 生成结构化输出
        if self.structured_output_fn is not None:
            output.structured_response = await self.structured_output_fn(messages)
        if self.output_cls is not None:
            output.structured_response = await generate_structured_response(...)

        return StopEvent(result=output)

    # 分发工具调用事件
    for tool_call in ev.tool_calls:
        ctx.send_event(ToolCall(...))
```

#### call_tool - 调用工具

**位置**: `base_agent.py:503-538`

```python
@step
async def call_tool(self, ctx: Context, ev: ToolCall) -> ToolCallResult:
    """Calls the tool and handles the result."""
    tools = await self.get_tools(ev.tool_name)
    tools_by_name = {tool.metadata.name: tool for tool in tools}

    if ev.tool_name not in tools_by_name:
        result = ToolOutput(content=f"Tool {ev.tool_name} not found.", is_error=True)
    else:
        tool = tools_by_name[ev.tool_name]
        result = await self._call_tool(ctx, tool, ev.tool_kwargs)

    return ToolCallResult(
        tool_name=ev.tool_name,
        tool_output=result,
        return_direct=tool.metadata.return_direct if tool else False,
    )
```

#### aggregate_tool_results - 聚合结果

**位置**: `multi_agent_workflow.py:561-639`

```python
@step
async def aggregate_tool_results(self, ctx: Context, ev: ToolCallResult):
    """Aggregate tool results and return the next agent input."""
    # 收集所有工具调用结果
    tool_call_results = ctx.collect_events(
        ev, expected=[ToolCallResult] * num_tool_calls
    )

    # 让当前代理处理工具结果
    await agent.handle_tool_call_results(ctx, tool_call_results, memory)

    # 检查是否需要切换代理 (handoff 设置的)
    next_agent_name = await ctx.store.get("next_agent", default=None)
    if next_agent_name:
        await ctx.store.set("current_agent_name", next_agent_name)
        await ctx.store.set("next_agent", None)

    # 检查是否有工具要求直接返回
    if any(r.return_direct and not r.tool_output.is_error for r in tool_call_results):
        if return_direct_tool.tool_name != "handoff":
            return StopEvent(result=result)

    # 继续下一轮迭代
    return AgentInput(input=input_messages, current_agent_name=agent.name)
```

---

## 4. 多引擎结果综合机制

### 4.1 共享内存

所有代理共享同一个 `ChatMemoryBuffer`：

**位置**: `multi_agent_workflow.py:251-258`

```python
async def _init_context(self, ctx: Context, ev: StartEvent) -> None:
    if not await ctx.store.get("memory", default=None):
        default_memory = ev.get("memory", default=None)
        default_memory = default_memory or ChatMemoryBuffer.from_defaults(
            llm=self.agents[self.root_agent].llm or Settings.llm
        )
        await ctx.store.set("memory", default_memory)
```

### 4.2 共享状态

通过 `initial_state` 和 `state_prompt` 实现状态传递：

**位置**: `multi_agent_workflow.py:269-270`

```python
if not await ctx.store.get("state", default=None):
    await ctx.store.set("state", self.initial_state)
```

**State Prompt 模板** (`prompts.py:8-13`):

```python
DEFAULT_STATE_PROMPT = """Current state:
{state}

Current message:
{msg}
"""
```

### 4.3 结构化输出

**位置**: `base_agent.py:450-484`

```python
# 方式1：自定义函数
if self.structured_output_fn is not None:
    if inspect.iscoroutinefunction(self.structured_output_fn):
        output.structured_response = await self.structured_output_fn(messages)
    else:
        output.structured_response = self.structured_output_fn(messages)

# 方式2：Pydantic 模型
if self.output_cls is not None:
    output.structured_response = await generate_structured_response(
        messages=llm_input,
        llm=self.llm,
        output_cls=self.output_cls
    )
```

---

## 5. FunctionAgent 实现

**位置**: `function_agent.py`

```python
class FunctionAgent(BaseWorkflowAgent):
    scratchpad_key: str = "scratchpad"  # 临时存储多轮对话
    allow_parallel_tool_calls: bool = True  # 支持并行工具调用

    async def take_step(self, ctx, llm_input, tools, memory) -> AgentOutput:
        # 合并历史消息 + scratchpad
        scratchpad = await ctx.store.get(self.scratchpad_key, default=[])
        current_llm_input = [*llm_input, *scratchpad]

        # 调用 LLM 的 function calling 能力
        response = await self.llm.achat_with_tools(
            chat_history=current_llm_input,
            tools=tools,
            allow_parallel_tool_calls=self.allow_parallel_tool_calls,
        )

        # 解析工具调用
        tool_calls = self.llm.get_tool_calls_from_response(response)
        scratchpad.append(response.message)

        return AgentOutput(
            response=response.message,
            tool_calls=tool_calls,
            current_agent_name=self.name
        )

    async def handle_tool_call_results(self, ctx, results, memory) -> None:
        """将工具结果添加到 scratchpad"""
        scratchpad = await ctx.store.get(self.scratchpad_key, default=[])

        for result in results:
            scratchpad.append(ChatMessage(
                role="tool",
                blocks=result.tool_output.blocks,
                additional_kwargs={"tool_call_id": result.tool_id},
            ))

        await ctx.store.set(self.scratchpad_key, scratchpad)

    async def finalize(self, ctx, output, memory) -> AgentOutput:
        """将 scratchpad 内容写入 memory 并清空"""
        scratchpad = await ctx.store.get(self.scratchpad_key, default=[])
        await memory.aput_messages(scratchpad)
        await ctx.store.set(self.scratchpad_key, [])
        return output
```

---

## 6. 完整流程示例

```
用户: "帮我查询天气，然后预订明天的航班"

        ┌─────────────────┐
        │  AgentWorkflow  │
        └────────┬────────┘
                 │
     ┌───────────▼───────────┐
     │    WeatherAgent       │  ← root_agent
     │  (查询天气能力)         │
     └───────────┬───────────┘
                 │ LLM判断: "我可以查天气"
                 │ 调用 weather_tool
                 ▼
     ┌───────────────────────┐
     │  tool: get_weather()  │
     └───────────┬───────────┘
                 │ 天气结果: "晴天 25°C"
                 │
                 │ LLM判断: "航班预订不是我的职责"
                 │ 调用 handoff(to_agent="BookingAgent")
                 ▼
     ┌───────────────────────┐
     │    BookingAgent       │  ← 切换后的代理
     │  (航班预订能力)         │
     └───────────┬───────────┘
                 │ 继承之前的 memory (包含天气信息)
                 │ 调用 book_flight_tool
                 ▼
     ┌───────────────────────┐
     │ tool: book_flight()   │
     └───────────┬───────────┘
                 │
                 ▼
         最终响应 (综合天气+航班结果)
```

---

## 7. 关键机制总结

| 机制 | 实现方式 | 关键代码位置 |
|------|---------|-------------|
| **多意图识别** | Handoff Tool + LLM 自主决策 | `multi_agent_workflow.py:57-76, 198-228` |
| **代理切换** | `next_agent` 状态 + `aggregate_tool_results` 检查 | `multi_agent_workflow.py:589-594` |
| **结果共享** | ChatMemoryBuffer (所有代理共享) | `base_agent.py:252-257` |
| **状态传递** | `initial_state` + `state_prompt` 注入 | `multi_agent_workflow.py:269-270` |
| **结构化输出** | `structured_output_fn` 或 `output_cls` | `base_agent.py:450-484` |
| **并行工具调用** | `allow_parallel_tool_calls` + `ctx.collect_events` | `function_agent.py:26-29` |
| **迭代控制** | `max_iterations` + `num_iterations` 计数 | `base_agent.py:412-423` |

---

## 8. 设计特点

### 优点

1. **灵活的意图路由**: 依赖 LLM 自主判断，无需预定义意图分类
2. **共享上下文**: 所有代理共享 memory，信息传递自然
3. **可扩展性**: 易于添加新的代理类型
4. **并行执行**: 支持多工具并行调用

### 局限

1. **依赖 LLM 能力**: 意图路由质量取决于 LLM 理解能力
2. **无显式意图分类**: 难以做意图统计和监控
3. **Token 消耗**: 每次都需要传递完整的代理描述信息
