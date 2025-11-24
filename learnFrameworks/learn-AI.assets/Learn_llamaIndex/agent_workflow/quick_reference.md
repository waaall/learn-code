# LLamaIndex Agent Workflow 快速参考

## 文件清单

### 核心文件 (workflow目录)

| 文件 | 行数 | 主要内容 | 关键类 |
|------|------|---------|--------|
| `__init__.py` | 30 | 模块导出 | - |
| `workflow_events.py` | 144 | 事件定义 | AgentInput/Output/Setup/Stream, ToolCall/Result |
| `base_agent.py` | 645 | 基础代理 | BaseWorkflowAgent (抽象基类, 6个@step方法) |
| `function_agent.py` | 195 | 函数调用 | FunctionAgent (scratchpad模式) |
| `react_agent.py` | 300 | ReAct推理 | ReActAgent (reasoning_key模式) |
| `codeact_agent.py` | 399 | 代码执行 | CodeActAgent (代码提取+执行) |
| `multi_agent_workflow.py` | 720 | 多代理 | AgentWorkflow (意图识别+代理切换) |
| `prompts.py` | 15 | 提示词 | DEFAULT_HANDOFF/STATE/HANDOFF_OUTPUT_PROMPT |

### 相关文件 (react目录)

| 文件 | 行数 | 主要内容 |
|------|------|---------|
| `react/types.py` | 76 | BaseReasoningStep, ActionStep, ObservationStep, ResponseStep |
| `react/formatter.py` | - | ReActChatFormatter (格式化推理步骤) |
| `react/output_parser.py` | - | ReActOutputParser (解析推理步骤) |
| `utils.py` | 42 | generate_structured_response, messages_to_xml_format |

## 继承关系速查表

```
┌─ Workflow (llama_index.core.workflow)
│  ├─ BaseWorkflowAgent
│  │  ├─ FunctionAgent
│  │  ├─ ReActAgent
│  │  └─ CodeActAgent
│  └─ AgentWorkflow

┌─ BaseModel (pydantic)
├─ BaseWorkflowAgent
└─ AgentWorkflow

┌─ PromptMixin
├─ BaseWorkflowAgent
└─ AgentWorkflow

┌─ Event (llama_index.core.workflow)
├─ AgentInput
├─ AgentSetup
├─ AgentStream
├─ AgentStreamStructuredOutput
├─ AgentOutput
├─ ToolCall
└─ ToolCallResult
```

## 6大工作流步骤

```python
@step
async def init_run(ctx, ev: AgentWorkflowStartEvent) -> AgentInput
    # 1. 初始化memory/state/消息
    # 2. 处理user_msg和chat_history

@step
async def setup_agent(ctx, ev: AgentInput) -> AgentSetup
    # 1. 添加system_prompt
    # 2. 格式化state信息
    # 3. 使用state_prompt

@step
async def run_agent_step(ctx, ev: AgentSetup) -> AgentOutput
    # 1. 获取工具
    # 2. 调用take_step() (子类实现)
    # 3. 流输出AgentOutput

@step
async def parse_agent_output(ctx, ev: AgentOutput) -> Union[StopEvent, AgentInput, ToolCall, None]
    # 1. 检查迭代限制
    # 2. 检查tool_calls
    # 3. 生成结构化输出

@step
async def call_tool(ctx, ev: ToolCall) -> ToolCallResult
    # 1. 查找工具
    # 2. 执行工具
    # 3. 返回结果

@step
async def aggregate_tool_results(ctx, ev: ToolCallResult) -> Union[AgentInput, StopEvent, None]
    # [关键多代理点] 处理next_agent切换
    # 1. 检查next_agent -> 更新current_agent_name
    # 2. 调用handle_tool_call_results()
    # 3. 处理return_direct
    # 4. 返回继续或停止
```

## 子类必需实现 (3个抽象方法)

```python
@abstractmethod
async def take_step(ctx, llm_input, tools, memory) -> AgentOutput:
    """执行一步推理/决策"""
    # FunctionAgent: 调用llm.achat_with_tools()
    # ReActAgent: 解析推理步骤
    # CodeActAgent: 提取和分析代码

@abstractmethod
async def handle_tool_call_results(ctx, results, memory) -> None:
    """处理工具调用结果"""
    # FunctionAgent: 添加到scratchpad
    # ReActAgent: 添加ObservationStep到reasoning
    # CodeActAgent: 添加到scratchpad

@abstractmethod
async def finalize(ctx, output, memory) -> AgentOutput:
    """最终化输出"""
    # 通常: 将中间状态添加到memory, 返回output
```

## 关键属性速查

### BaseWorkflowAgent属性
```
name                      # 代理名称
description              # 代理描述 (用于handoff_prompt)
system_prompt            # 系统提示词
tools                    # 可用工具列表
tool_retriever          # 动态工具检索器
can_handoff_to          # 可切换到的代理列表 (多代理特性)
llm                     # 大语言模型
initial_state           # 初始状态 Dict
state_prompt            # 状态格式化提示词
output_cls              # 输出类型 (Pydantic BaseModel)
structured_output_fn    # 自定义结构化输出函数
streaming               # 是否流式输出
```

### FunctionAgent特有属性
```
scratchpad_key = "scratchpad"           # 存储键
initial_tool_choice = None              # 首次工具选择
allow_parallel_tool_calls = True        # 并行工具调用
```

### ReActAgent特有属性
```
reasoning_key = "current_reasoning"     # 存储键
output_parser = ReActOutputParser()     # 输出解析器
formatter = ReActChatFormatter()        # 格式化器
```

### CodeActAgent特有属性
```
scratchpad_key = "scratchpad"           # 存储键
code_execute_fn                         # 代码执行函数 (必需!)
code_act_system_prompt                  # CodeAct系统提示词
```

### AgentWorkflow特有属性
```
agents: List[BaseWorkflowAgent]         # 代理列表
root_agent: str                         # 根代理名称
handoff_prompt                          # 切换提示词模板
handoff_output_prompt                   # 切换输出提示词
state_prompt                            # 状态提示词
output_cls                              # 输出类型
structured_output_fn                    # 结构化输出函数
initial_state                           # 共享初始状态
```

## 关键存储键 (ctx.store)

```python
# 通用键
"memory"                    # ChatMemoryBuffer
"state"                     # Dict[str, Any]
"num_iterations"            # int
"max_iterations"            # int
"user_msg_str"              # str
"formatted_input_with_state"# bool
"current_tool_calls"        # List[ToolCallResult]
"num_tool_calls"            # int

# 多代理键
"current_agent_name"        # str (当前代理)
"next_agent"                # Optional[str] (由handoff设置)
"agents"                    # List[str]
"can_handoff_to"            # Dict[str, Optional[List[str]]]
"handoff_output_prompt"     # Union[str, PromptTemplate]

# 代理特定键 (动态生成)
f"{agent.scratchpad_key}"   # FunctionAgent/CodeActAgent
f"{agent.reasoning_key}"    # ReActAgent
```

## 事件流转关系

```
StartEvent
  ↓
AgentInput (init_run输出)
  ↓
AgentSetup (setup_agent输出)
  ├→ AgentStream (run_agent_step流输出)
  ↓
AgentOutput (run_agent_step输出)
  ├→ AgentStreamStructuredOutput (parse_agent_output)
  ├→ ToolCall (有tool_calls时)
  │   ↓
  │   ToolCallResult (call_tool输出)
  │   ↓
  │   AgentInput (aggregate_tool_results继续)
  │   └→ [回到parse_agent_output]
  │
  └→ StopEvent (完成)
```

## 多意图识别的关键流程

### 1. Handoff工具创建
```python
# AgentWorkflow._get_handoff_tool()
# 为每个当前代理创建handoff工具
# 工具描述包含: agent_info = {agent.name: agent.description}
# 返回: FunctionTool(async_fn=handoff, return_direct=True)
```

### 2. Handoff函数调用
```python
# AgentWorkflow.handoff(ctx, to_agent, reason)
# 验证to_agent有效性
# 检查can_handoff_to权限
# 设置ctx.store["next_agent"] = to_agent
# 返回用户友好的确认消息
```

### 3. 代理切换
```python
# AgentWorkflow.aggregate_tool_results()
next_agent = await ctx.store.get("next_agent")
if next_agent:
    await ctx.store.set("current_agent_name", next_agent)
    await ctx.store.set("next_agent", None)
```

## 结构化输出的两种方式

### 方式1: 使用output_cls
```python
agent = FunctionAgent(
    name="calc",
    output_cls=CalculationResult,  # 自动生成
    tools=[...],
    llm=...
)

# 内部调用:
output.structured_response = await generate_structured_response(
    messages=llm_input, 
    llm=self.llm, 
    output_cls=self.output_cls
)
# 使用XML格式化消息 + as_structured_llm()
```

### 方式2: 使用structured_output_fn
```python
def my_extractor(messages: List[ChatMessage]) -> Dict[str, Any]:
    # 自定义逻辑
    return {...}

agent = FunctionAgent(
    name="calc",
    structured_output_fn=my_extractor,  # 自定义函数
    tools=[...],
    llm=...
)

# 内部调用:
output.structured_response = await self.structured_output_fn(messages)
# 支持async和sync
```

## 常见错误和解决方案

| 问题 | 原因 | 解决方案 |
|------|------|--------|
| Max iterations exceeded | 无限循环 | 增加max_iterations或修复代理逻辑 |
| Tool not found | 工具名不对 | 检查tool.metadata.name |
| Handoff failed | 目标代理不存在或权限问题 | 检查can_handoff_to配置 |
| Streaming not working | LLM不支持或streaming=False | 设置streaming=True, 使用支持的LLM |
| Empty response | LLM返回空消息 | 检查system_prompt和LLM配置 |
| Structured output failed | output_cls验证失败 | 检查LLM输出是否符合schema |

## 性能优化建议

1. **减少LLM调用**:
   - 批量工具调用 (FunctionAgent)
   - 合理设计state_prompt

2. **控制迭代**:
   - 设置合理max_iterations (默认20)
   - 使用return_direct工具提前返回

3. **内存优化**:
   - 定期清理memory (ChatMemoryBuffer有max_tokens)
   - 使用state压缩存储

4. **并发优化**:
   - FunctionAgent支持allow_parallel_tool_calls=True
   - 多工作流运行使用asyncio

5. **流式输出**:
   - streaming=True获得更好的用户体验
   - 使用event_stream()逐个处理事件

## 调试技巧

```python
# 1. 启用verbose日志
agent = FunctionAgent(..., verbose=True)

# 2. 监听事件流
handler = agent.run(user_msg="...")
for event in handler.event_stream():
    print(f"Event: {type(event).__name__}: {event}")

# 3. 检查上下文存储
async def debug_ctx(ctx):
    memory = await ctx.store.get("memory")
    state = await ctx.store.get("state")
    print(f"Memory: {memory}")
    print(f"State: {state}")

# 4. 自定义系统提示词增加调试信息
system_prompt = """
你是一个有用的助手.
[DEBUG] 输出你的思考过程
"""

# 5. 使用CodeActAgent测试工具
# 在代码执行中可以打印调试信息
```

## 选择合适的Agent类型

| Agent类型 | 优点 | 缺点 | 适用场景 |
|----------|------|------|---------|
| **FunctionAgent** | 利用LLM函数调用能力, 速度快 | 需要LLM支持函数调用 | 大多数工具调用任务 |
| **ReActAgent** | 完整推理链, 可解释性好 | 速度较慢, 解析开销大 | 复杂推理, 需要可解释性 |
| **CodeActAgent** | 灵活执行代码, 功能强大 | 安全风险, 需要代码执行环境 | 数据分析, 代码生成 |

## 多代理系统设计建议

1. **代理粒度**:
   - 每个代理负责一个明确的能力
   - 保持代理数量 (3-10个较好)

2. **切换策略**:
   - Root Agent作为调度器
   - 清晰的handoff_prompt

3. **状态传递**:
   - 使用shared state存储公共信息
   - 通过memory传递详细对话历史

4. **错误处理**:
   - 在handoff()中验证代理可用性
   - 提供回退机制

5. **测试**:
   - 测试单个代理独立功能
   - 测试多代理交互场景

