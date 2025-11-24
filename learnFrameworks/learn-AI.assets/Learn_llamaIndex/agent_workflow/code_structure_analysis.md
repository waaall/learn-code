# LLamaIndex Agent Workflow 代码结构分析

## 目录总览
```
/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/
├── __init__.py                    (模块导出)
├── workflow_events.py             (事件定义)
├── base_agent.py                  (基础代理类)
├── function_agent.py              (函数调用代理)
├── react_agent.py                 (ReAct推理代理)
├── codeact_agent.py               (代码执行代理)
├── multi_agent_workflow.py        (多代理工作流)
└── prompts.py                     (提示词模板)
```

## 1. 整体文件组织和模块结构

### 1.1 核心模块导出 (__init__.py)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/__init__.py`

导出的主要类和事件:
- AgentWorkflow: 多代理工作流管理器
- BaseWorkflowAgent: 所有代理的基类
- FunctionAgent: 函数调用代理
- ReActAgent: ReAct推理代理
- CodeActAgent: 代码执行代理
- 事件类: AgentInput, AgentOutput, AgentSetup, AgentStream, ToolCall, ToolCallResult, AgentStreamStructuredOutput

### 1.2 事件系统 (workflow_events.py)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/workflow_events.py`
**行数**: 144

关键事件定义:

```python
class AgentInput(Event):
    """LLM输入事件"""
    input: list[ChatMessage]
    current_agent_name: str

class AgentSetup(Event):
    """代理设置事件"""
    input: list[ChatMessage]
    current_agent_name: str

class AgentStream(Event):
    """代理流输出事件 (用于流式输出)"""
    delta: str
    response: str
    current_agent_name: str
    tool_calls: list[ToolSelection]
    thinking_delta: Optional[str]

class AgentStreamStructuredOutput(Event):
    """结构化输出事件"""
    output: Dict[str, Any]

class AgentOutput(Event):
    """LLM输出事件"""
    response: ChatMessage
    structured_response: Optional[Dict[str, Any]]
    tool_calls: list[ToolSelection]
    retry_messages: list[ChatMessage]

class ToolCall(Event):
    """工具调用事件"""
    tool_name: str
    tool_kwargs: dict
    tool_id: str

class ToolCallResult(Event):
    """工具调用结果事件"""
    tool_name: str
    tool_kwargs: dict
    tool_id: str
    tool_output: ToolOutput
    return_direct: bool

class AgentWorkflowStartEvent(StartEvent):
    """工作流启动事件 (带自动验证)"""
```

## 2. 关键类和它们的继承关系

### 2.1 继承层级关系

```
Workflow (来自 llama_index.core.workflow)
├── BaseWorkflowAgent
│   ├── FunctionAgent
│   ├── ReActAgent
│   └── CodeActAgent
└── AgentWorkflow

BaseModel + PromptMixin
├── BaseWorkflowAgent
└── AgentWorkflow
```

### 2.2 BaseWorkflowAgent (基础代理类)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/base_agent.py`
**行数**: 645
**元类**: BaseWorkflowAgentMeta (组合 WorkflowMeta 和 ModelMetaclass)

**主要属性**:
```python
class BaseWorkflowAgent(Workflow, BaseModel, PromptMixin):
    name: str                                   # 代理名称
    description: str                           # 代理描述
    system_prompt: Optional[str]                # 系统提示词
    tools: Optional[List[Union[BaseTool, Callable]]]  # 可用工具
    tool_retriever: Optional[ObjectRetriever]  # 工具检索器
    can_handoff_to: Optional[List[str]]         # 可以切换到的代理
    llm: LLM                                    # 大语言模型
    initial_state: Dict[str, Any]               # 初始状态
    state_prompt: Union[str, BasePromptTemplate] # 状态更新提示词
    output_cls: Optional[Type[BaseModel]]       # 输出类型
    structured_output_fn: Optional[Callable]    # 结构化输出函数
    streaming: bool                             # 是否流式输出
```

**抽象方法** (子类必须实现):
```python
@abstractmethod
async def take_step(ctx, llm_input, tools, memory) -> AgentOutput
    """执行一个代理步骤"""

@abstractmethod
async def handle_tool_call_results(ctx, results, memory) -> None
    """处理工具调用结果"""

@abstractmethod
async def finalize(ctx, output, memory) -> AgentOutput
    """最终化代理输出"""
```

**工作流步骤** (@step 装饰):

1. **init_run** (行309)
   - 初始化上下文
   - 处理用户消息和聊天历史
   - 返回 AgentInput

2. **setup_agent** (行363)
   - 准备LLM输入
   - 添加系统提示词
   - 使用state_prompt更新状态信息
   - 返回 AgentSetup

3. **run_agent_step** (行391)
   - 获取工具
   - 调用 take_step() 执行代理
   - 流写输出事件
   - 返回 AgentOutput

4. **parse_agent_output** (行408)
   - 检查迭代限制
   - 处理retry_messages
   - 生成结构化输出
   - 返回 StopEvent, AgentInput, ToolCall 或 None

5. **call_tool** (行503)
   - 执行工具调用
   - 返回 ToolCallResult

6. **aggregate_tool_results** (行540)
   - 汇总工具结果
   - 处理 return_direct 标志
   - 返回 AgentInput, StopEvent 或 None

### 2.3 FunctionAgent (函数调用代理)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/function_agent.py`
**行数**: 195

**专有属性**:
```python
scratchpad_key: str = "scratchpad"                    # 草稿板存储键
initial_tool_choice: Optional[str] = None             # 初始工具选择
allow_parallel_tool_calls: bool = True                # 是否允许并行工具调用
```

**核心实现**:
- `_get_response()`: 调用 `llm.achat_with_tools()` 获取函数调用
- `_get_streaming_response()`: 流式调用 `llm.astream_chat_with_tools()`
- `take_step()`: 
  * 获取并管理scratchpad (多轮对话缓冲)
  * 调用LLM获得工具调用
  * 返回AgentOutput (包含tool_calls)
- `handle_tool_call_results()`: 将工具结果添加到scratchpad
- `finalize()`: 将scratchpad消息添加到内存

**特点**:
- 使用LLM原生函数调用能力
- 支持并行工具调用
- 利用scratchpad进行多轮对话管理

### 2.4 ReActAgent (ReAct推理代理)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/react_agent.py`
**行数**: 300

**专有属性**:
```python
reasoning_key: str = "current_reasoning"              # 推理步骤存储键
output_parser: ReActOutputParser                      # ReAct输出解析器
formatter: ReActChatFormatter                         # ReAct聊天格式化器
```

**关键类型** (来自 react/types.py):
```python
BaseReasoningStep (抽象基类)
├── ActionReasoningStep
│   thought: str
│   action: str
│   action_input: Dict
├── ObservationReasoningStep
│   observation: str
│   return_direct: bool = False
└── ResponseReasoningStep
    thought: str
    response: str
    is_streaming: bool = False
```

**核心实现**:
- `take_step()`:
  * 使用formatter格式化聊天历史和推理步骤
  * 调用LLM获得响应
  * 使用output_parser解析推理步骤
  * 创建ActionReasoningStep用于工具调用
  * 创建ResponseReasoningStep用于最终回答
  
- `handle_tool_call_results()`:
  * 为每个工具结果创建ObservationReasoningStep
  * 累积推理步骤序列

- `finalize()`:
  * 处理ResponseReasoningStep
  * 将推理链添加到内存
  * 从最终响应移除"Answer:"前缀

**特点**:
- 基于思维链(Chain of Thought)推理
- 完整的推理步骤跟踪
- 支持长文本推理和流式输出

### 2.5 CodeActAgent (代码执行代理)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/codeact_agent.py`
**行数**: 399

**专有属性**:
```python
scratchpad_key: str = "scratchpad"
code_execute_fn: Union[Callable, Awaitable]           # 代码执行函数 (必需)
code_act_system_prompt: Union[str, BasePromptTemplate] # CodeAct系统提示词
```

**默认系统提示词**:
```python
DEFAULT_CODE_ACT_PROMPT = """你是一个有用的AI助手，可以编写并执行Python代码...
<execute>...</execute> 标签中放置代码
变量在顶级代码中定义可在后续代码中引用
"""
```

**核心实现**:
- `_extract_code_from_response()`: 从响应中提取 `<execute>...</execute>` 中的代码
- `_get_tool_descriptions()`: 使用反射生成工具描述
- `_get_tool_fns()`: 验证并收集工具函数
- `take_step()`:
  * 获取scratchpad
  * 动态生成系统提示词 (包含tool_descriptions)
  * 从LLM响应中提取代码
  * 为提取的代码创建ToolCall
  * 支持handoff工具
  
- `handle_tool_call_results()`:
  * 区分处理 EXECUTE_TOOL_NAME 和 handoff 工具
  * 将结果作为用户消息添加到scratchpad

- `finalize()`: 将scratchpad消息添加到内存

**特点**:
- 支持动态代码执行
- XML标签提取代码
- 完整的工具集成
- 推理和执行一体化

### 2.6 AgentWorkflow (多代理工作流)
**文件路径**: `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/multi_agent_workflow.py`
**行数**: 720
**元类**: AgentWorkflowMeta (组合 WorkflowMeta 和 ABCMeta)

**关键特性**:

#### 构造器参数:
```python
agents: List[BaseWorkflowAgent]                      # 多个代理
initial_state: Optional[Dict] = None                 # 共享初始状态
root_agent: Optional[str] = None                     # 根代理名称
handoff_prompt: Optional[Union[str, BasePromptTemplate]] # 切换提示词
handoff_output_prompt: Optional[Union[str, BasePromptTemplate]] # 切换输出提示词
state_prompt: Optional[Union[str, BasePromptTemplate]] # 状态更新提示词
output_cls: Optional[Type[BaseModel]] = None         # 输出类型
structured_output_fn: Optional[Callable] = None      # 结构化输出函数
```

#### 多意图识别和代理选择机制:

```python
async def _get_handoff_tool(current_agent) -> Optional[AsyncBaseTool]:
    """为当前代理创建切换工具
    - 过滤出当前代理及其can_handoff_to限制的代理
    - 生成agent_info (名称->描述映射)
    - 使用handoff_prompt格式化
    - 返回带有切换函数的FunctionTool
    """
```

**关键意图识别流程**:
1. 根代理收到用户请求
2. 根代理可以调用 `handoff()` 工具来识别最合适的代理
3. handoff_prompt中包含所有可用代理的描述
4. LLM基于代理描述选择合适的代理

**handoff函数** (行57):
```python
async def handoff(ctx: Context, to_agent: str, reason: str) -> str:
    """
    检查目标代理是否有效
    检查可切换性约束 (can_handoff_to)
    设置ctx.store["next_agent"]
    返回用户友好的消息
    """
```

**工作流步骤** (@step 装饰):

1. **init_run** (行322): 初始化多代理上下文
   - 初始化agents列表
   - 初始化can_handoff_to映射
   - 设置当前代理名为root_agent

2. **setup_agent** (行377): 准备当前代理的输入
   - 获取当前代理
   - 应用该代理的系统提示词

3. **run_agent_step** (行407): 运行当前代理
   - 调用当前代理的take_step()
   - 获取当前代理的工具 (包括handoff工具)

4. **parse_agent_output** (行425): 解析代理输出
   - 处理retry_messages
   - 生成结构化输出

5. **call_tool** (行523): 执行工具
   - 获取当前代理的工具

6. **aggregate_tool_results** (line 561): **关键步骤**
   - 收集所有工具结果
   - **检查next_agent (handoff触发的代理切换)**:
     ```python
     next_agent_name = await ctx.store.get("next_agent", default=None)
     if next_agent_name:
         await ctx.store.set("current_agent_name", next_agent_name)
         await ctx.store.set("next_agent", None)
     ```
   - 调用当前代理的handle_tool_call_results()
   - 处理return_direct工具结果

#### 多代理结果综合处理:

```python
# 在parse_agent_output中
if self.structured_output_fn is not None:
    output.structured_response = await self.structured_output_fn(messages)
    
if self.output_cls is not None:
    output.structured_response = await generate_structured_response(
        messages=llm_input, llm=agent.llm, output_cls=self.output_cls
    )
```

**特点**:
- 支持多代理系统
- 基于代理描述的智能切换
- 共享内存 (所有代理使用同一memory)
- 可配置的切换约束 (can_handoff_to)

## 3. 多意图识别相关的代码

### 3.1 识别机制概述

LLamaIndex的多意图识别主要通过以下方式实现:

**方式1: 基于代理描述的智能切换 (推荐)**
```
用户请求 
  ↓
Root Agent 接收请求
  ↓
Root Agent的系统提示词包含handoff工具描述
  ↓
handoff_prompt格式化所有可用代理的描述
  ↓
LLM识别最合适的代理
  ↓
调用handoff(to_agent=best_agent, reason=...)
  ↓
切换到目标代理继续处理
```

### 3.2 关键代码片段

**1. Handoff工具创建 (base_agent.py, multi_agent_workflow.py)**
```python
# multi_agent_workflow.py, 行198-228
def _get_handoff_tool(self, current_agent: BaseWorkflowAgent) -> Optional[AsyncBaseTool]:
    """创建handoff工具"""
    agent_info = {cfg.name: cfg.description for cfg in self.agents.values()}
    
    # 过滤不可切换的代理
    for name in agent_info:
        if name == current_agent.name:
            configs_to_remove.append(name)
        elif current_agent.can_handoff_to is not None:
            if name not in current_agent.can_handoff_to:
                configs_to_remove.append(name)
    
    # 生成handoff提示词
    fn_tool_prompt = self.handoff_prompt.format(agent_info=str(agent_info))
    return FunctionTool.from_defaults(
        async_fn=handoff, description=fn_tool_prompt, return_direct=True
    )
```

**2. 代理切换逻辑 (multi_agent_workflow.py, 行589-594)**
```python
# 在aggregate_tool_results中
next_agent_name = await ctx.store.get("next_agent", default=None)
if next_agent_name:
    # 更新当前代理
    await ctx.store.set("current_agent_name", next_agent_name)
    await ctx.store.set("next_agent", None)
```

**3. Handoff函数实现 (multi_agent_workflow.py, 行57-76)**
```python
async def handoff(ctx: Context, to_agent: str, reason: str) -> str:
    """执行代理切换"""
    agents: list[str] = await ctx.store.get("agents")
    current_agent_name: str = await ctx.store.get("current_agent_name")
    can_handoff_to: dict = await ctx.store.get("can_handoff_to")
    
    # 验证目标代理
    if to_agent not in agents:
        return f"代理{to_agent}不存在..."
    
    # 验证切换权限
    if can_handoff_to.get(current_agent_name) is not None:
        if to_agent not in can_handoff_to.get(current_agent_name, []):
            return f"不能从{current_agent_name}切换到{to_agent}..."
    
    # 设置下一个代理
    await ctx.store.set("next_agent", to_agent)
    
    # 返回切换确认消息
    handoff_output_prompt = await ctx.store.get(
        "handoff_output_prompt", default=DEFAULT_HANDOFF_OUTPUT_PROMPT
    )
    return handoff_output_prompt.format(to_agent=to_agent, reason=reason)
```

### 3.3 意图识别流程图

```
AgentWorkflow.init_run()
  │
  ├─ 初始化agents列表
  └─ 设置current_agent_name = root_agent
  
Root Agent 收到请求
  │
  ├─ Root Agent.setup_agent()
  ├─ Root Agent.run_agent_step()
  │   └─ 使用take_step()
  │       └─ LLM识别意图 (通过handoff_prompt)
  │
  └─ parse_agent_output()
      └─ 检查tool_calls
          ├─ 如果包含handoff工具调用
          │   ├─ call_tool() 执行handoff
          │   └─ handoff() 验证并设置next_agent
          │
          └─ aggregate_tool_results()
              ├─ 检查next_agent
              ├─ 如果next_agent非空，更新current_agent_name
              └─ 返回AgentInput给新代理继续处理
```

### 3.4 支持的意图识别维度

1. **基于代理描述**: 通过agent.description
2. **基于工具集**: 每个代理拥有不同的tools
3. **基于能力范围**: agent.can_handoff_to 限制代理交互
4. **共享状态**: 所有代理可以访问共享的state和memory
5. **渐进式处理**: 多个代理可以顺序处理同一请求

## 4. 多引擎结果综合处理相关的代码

### 4.1 结果综合处理机制

LLamaIndex支持多引擎(多代理)结果综合处理，主要在以下两个层面:

#### 层面1: 单代理内的结果综合
- FunctionAgent: 使用scratchpad管理多轮工具调用
- ReActAgent: 使用reasoning_key管理推理链
- CodeActAgent: 使用scratchpad + code_execute_fn

#### 层面2: 多代理之间的结果综合
- 共享memory (ChatMemoryBuffer): 所有代理都能访问
- 共享state: initial_state
- 结构化输出: output_cls / structured_output_fn

### 4.2 关键代码片段

**1. 结构化输出处理 (base_agent.py, 行450-484)**
```python
# 使用structured_output_fn生成结构化输出
if self.structured_output_fn is not None:
    try:
        if inspect.iscoroutinefunction(self.structured_output_fn):
            output.structured_response = await self.structured_output_fn(messages)
        else:
            output.structured_response = cast(Dict[str, Any], self.structured_output_fn(messages))
        ctx.write_event_to_stream(
            AgentStreamStructuredOutput(output=output.structured_response)
        )
    except Exception as e:
        warnings.warn(f"结构化输出生成问题: {e}")

# 或使用output_cls自动生成
if self.output_cls is not None:
    try:
        llm_input = [*messages]
        if self.system_prompt:
            llm_input = [ChatMessage(role="system", content=self.system_prompt), *llm_input]
        output.structured_response = await generate_structured_response(
            messages=llm_input, llm=self.llm, output_cls=self.output_cls
        )
        ctx.write_event_to_stream(
            AgentStreamStructuredOutput(output=output.structured_response)
        )
    except Exception as e:
        warnings.warn(f"结构化输出生成问题: {e}")
```

**2. 工具结果聚合 (base_agent.py, 行541-604)**
```python
async def aggregate_tool_results(self, ctx, ev) -> Union[AgentInput, StopEvent, None]:
    """工具结果聚合"""
    num_tool_calls = await ctx.store.get("num_tool_calls", default=0)
    tool_call_results: list[ToolCallResult] = ctx.collect_events(
        ev, expected=[ToolCallResult] * num_tool_calls
    )
    
    # 跟踪所有工具调用
    cur_tool_calls: List[ToolCallResult] = await ctx.store.get("current_tool_calls", default=[])
    cur_tool_calls.extend(tool_call_results)
    await ctx.store.set("current_tool_calls", cur_tool_calls)
    
    # 处理工具结果
    await self.handle_tool_call_results(ctx, tool_call_results, memory)
    
    # 处理return_direct (某些工具可以直接返回结果)
    if any(tcr.return_direct and not tcr.tool_output.is_error for tcr in tool_call_results):
        return_direct_tool = next(tcr for tcr in tool_call_results if tcr.return_direct and not tcr.tool_output.is_error)
        
        # 最终化输出
        result = await self.finalize(ctx, result, memory)
        
        # 如果不是handoff工具，停止处理
        if return_direct_tool.tool_name != "handoff":
            return StopEvent(result=result)
    
    # 继续迭代
    return AgentInput(input=input_messages, current_agent_name=self.name)
```

**3. 多代理结果综合 (multi_agent_workflow.py, 行589-639)**
```python
async def aggregate_tool_results(self, ctx, ev) -> Union[AgentInput, StopEvent, None]:
    """多代理工具结果聚合"""
    # ... (收集工具结果)
    
    # 关键: 处理代理切换
    next_agent_name = await ctx.store.get("next_agent", default=None)
    if next_agent_name:
        # 代理切换 (意图识别结果)
        await ctx.store.set("current_agent_name", next_agent_name)
        await ctx.store.set("next_agent", None)
    
    # 处理工具结果
    agent: BaseWorkflowAgent = self.agents[agent_name]
    await agent.handle_tool_call_results(ctx, tool_call_results, memory)
    
    # ... (继续处理或返回)
```

### 4.3 内存和状态管理

```python
# 共享内存 (所有代理可访问)
memory: BaseMemory = await ctx.store.get("memory")
# ChatMemoryBuffer支持:
# - aset(messages): 设置消息
# - aput(message): 添加单个消息
# - aput_messages(messages): 添加多个消息
# - aget(): 获取所有消息

# 共享状态 (初始化并在代理间传递)
state = await ctx.store.get("state", default=None)
# state_prompt用于在每个代理步骤中更新状态上下文
```

### 4.4 工具结果处理差异

**FunctionAgent** (function_agent.py, 行147-177):
```python
async def handle_tool_call_results(self, ctx, results, memory):
    """处理工具结果到scratchpad"""
    scratchpad: List[ChatMessage] = await ctx.store.get(self.scratchpad_key, default=[])
    for tool_call_result in results:
        # 添加工具响应消息
        scratchpad.append(ChatMessage(role="tool", blocks=tool_call_result.tool_output.blocks))
        # 处理return_direct
        if tool_call_result.return_direct and tool_call_result.tool_name != "handoff":
            scratchpad.append(ChatMessage(role="assistant", content=str(tool_call_result.tool_output.content)))
            break
    await ctx.store.set(self.scratchpad_key, scratchpad)
```

**ReActAgent** (react_agent.py, 行234-261):
```python
async def handle_tool_call_results(self, ctx, results, memory):
    """处理工具结果到推理链"""
    current_reasoning: list[BaseReasoningStep] = await ctx.store.get(self.reasoning_key, default=[])
    for tool_call_result in results:
        # 创建观察步骤
        obs_step = ObservationReasoningStep(
            observation=str(tool_call_result.tool_output.content),
            return_direct=tool_call_result.return_direct,
        )
        current_reasoning.append(obs_step)
        # 处理return_direct
        if tool_call_result.return_direct and tool_call_result.tool_name != "handoff":
            current_reasoning.append(ResponseReasoningStep(...))
            break
    await ctx.store.set(self.reasoning_key, current_reasoning)
```

**CodeActAgent** (codeact_agent.py, 行351-381):
```python
async def handle_tool_call_results(self, ctx, results, memory):
    """处理工具结果到scratchpad"""
    scratchpad: List[ChatMessage] = await ctx.store.get(self.scratchpad_key, default=[])
    for tool_call_result in results:
        if tool_call_result.tool_name == EXECUTE_TOOL_NAME:
            # 代码执行结果
            code_result = f"执行代码的结果:\n\n{tool_call_result.tool_output.content}"
            scratchpad.append(ChatMessage(role="user", content=code_result))
        elif tool_call_result.tool_name == "handoff":
            # handoff工具结果
            scratchpad.append(ChatMessage(role="tool", blocks=tool_call_result.tool_output.blocks))
    await ctx.store.set(self.scratchpad_key, scratchpad)
```

### 4.5 输出汇总流程

```
所有工具调用完成
  │
  └─ parse_agent_output() 
      │
      ├─ 如果有tool_calls: 跳过
      │
      └─ 如果没有tool_calls: (最终输出)
          │
          ├─ await agent.finalize() 
          │   └─ 将scratchpad/reasoning添加到内存
          │
          ├─ await memory.aget() 获取完整消息历史
          │
          ├─ 如果output_cls: 生成结构化输出
          │   └─ await generate_structured_response(messages, llm, output_cls)
          │       └─ messages_to_xml_format() 转换为XML
          │       └─ llm.as_structured_llm().achat() 调用
          │
          ├─ 或如果structured_output_fn: 调用自定义函数
          │
          ├─ 创建 AgentStreamStructuredOutput 事件
          │
          └─ 返回 StopEvent(result=output)
```

### 4.6 事件流输出

关键事件类型用于结果综合:

```python
AgentOutput: 代理单步输出 (response, tool_calls, structured_response)
ToolCall: 工具调用事件
ToolCallResult: 工具调用结果
AgentStream: 流式输出 (delta, response, tool_calls)
AgentStreamStructuredOutput: 结构化输出 (output)
StopEvent: 最终结果
```

## 5. 关键文件路径总结

| 功能 | 文件路径 | 行数 |
|-----|--------|------|
| 工作流事件定义 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/workflow_events.py` | 144 |
| 基础代理类 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/base_agent.py` | 645 |
| 函数调用代理 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/function_agent.py` | 195 |
| ReAct代理 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/react_agent.py` | 300 |
| 代码执行代理 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/codeact_agent.py` | 399 |
| 多代理工作流 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/multi_agent_workflow.py` | 720 |
| 提示词模板 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/workflow/prompts.py` | 15 |
| 工具类型定义 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/react/types.py` | 76 |
| 代理工具函数 | `/Users/zhengxu/Desktop/some_code/llama_index/llama-index-core/llama_index/core/agent/utils.py` | 42 |

