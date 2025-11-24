# LLamaIndex Agent Workflow 架构图和使用示例

## 系统架构图

### 整体架构

```
┌─────────────────────────────────────────────────────────────────┐
│                      AgentWorkflow                              │
│                 (多代理工作流管理器)                            │
└──────────────────┬──────────────────────────────────────────────┘
                   │
        ┌──────────┼──────────┐
        │          │          │
        v          v          v
    ┌────────┐  ┌────────┐  ┌────────┐
    │ Agent1 │  │ Agent2 │  │ Agent3 │  ... (多个BaseWorkflowAgent)
    │(Root)  │  │        │  │        │
    └────────┘  └────────┘  └────────┘
        │          │          │
        └──────────┼──────────┘
                   │
    ┌──────────────┴──────────────┐
    │                             │
    v                             v
共享内存                        共享状态
(ChatMemoryBuffer)             (Dict)


BaseWorkflowAgent的具体实现:
┌─────────────────────────────────────────────────────────────┐
│              BaseWorkflowAgent (抽象基类)                   │
│                                                             │
│ 核心方法:                                                   │
│  - take_step()                 (子类实现)                  │
│  - handle_tool_call_results()  (子类实现)                  │
│  - finalize()                  (子类实现)                  │
│  - init_run()                  (工作流步骤)                │
│  - setup_agent()               (工作流步骤)                │
│  - run_agent_step()            (工作流步骤)                │
│  - parse_agent_output()        (工作流步骤)                │
│  - call_tool()                 (工作流步骤)                │
│  - aggregate_tool_results()    (工作流步骤)                │
└─────────────────────────────────────────────────────────────┘
        │          │           │
        v          v           v
    ┌──────────┐ ┌────────┐ ┌──────────┐
    │Function  │ │ReAct   │ │CodeAct   │
    │Agent     │ │Agent   │ │Agent     │
    └──────────┘ └────────┘ └──────────┘
        │          │           │
        v          v           v
  scratchpad   reasoning_key  scratchpad
  (多轮)       (推理链)       (代码执行)
```

### 工作流执行流程

```
StartEvent (user_msg, chat_history, max_iterations)
    │
    v
┌─────────────────────────────────────────┐
│ AgentWorkflow.init_run()                │
│ BaseWorkflowAgent.init_run()            │
├─────────────────────────────────────────┤
│ 初始化context/memory/state              │
│ 返回: AgentInput                         │
└─────────────────────────────────────────┘
    │
    v
┌─────────────────────────────────────────┐
│ AgentWorkflow.setup_agent()             │
│ BaseWorkflowAgent.setup_agent()         │
├─────────────────────────────────────────┤
│ 添加system_prompt                       │
│ 格式化状态信息                          │
│ 返回: AgentSetup                        │
└─────────────────────────────────────────┘
    │
    v
┌─────────────────────────────────────────┐
│ AgentWorkflow.run_agent_step()          │
│ BaseWorkflowAgent.run_agent_step()      │
├─────────────────────────────────────────┤
│ 调用take_step() (由子类实现)            │
│ 返回: AgentOutput                       │
└─────────────────────────────────────────┘
    │
    v
┌─────────────────────────────────────────┐
│ AgentWorkflow.parse_agent_output()      │
│ BaseWorkflowAgent.parse_agent_output()  │
├─────────────────────────────────────────┤
│ 检查tool_calls                          │
│ ├─ 如果有tool_calls: 返回None          │
│ │   (继续到call_tool)                   │
│ │                                       │
│ └─ 如果没有tool_calls:                 │
│     ├─ 调用finalize()                  │
│     ├─ 生成结构化输出                   │
│     └─ 返回StopEvent (完成)            │
└─────────────────────────────────────────┘
    │
    ├─── (有tool_calls时)
    │         │
    │         v
    │    ┌──────────────────────────────┐
    │    │ AgentWorkflow.call_tool()    │
    │    ├──────────────────────────────┤
    │    │ 查找工具                     │
    │    │ 执行工具                     │
    │    │ 返回: ToolCallResult         │
    │    └──────────────────────────────┘
    │         │
    │         v
    │    ┌──────────────────────────────┐
    │    │aggregate_tool_results()      │
    │    ├──────────────────────────────┤
    │    │ [MULTI-AGENT关键点]         │
    │    │ 检查next_agent               │
    │    │ 更新current_agent_name       │
    │    │                              │
    │    │ 调用handle_tool_call_results()
    │    │ 检查return_direct            │
    │    │ 返回: AgentInput或StopEvent  │
    │    └──────────────────────────────┘
    │         │
    │         └──> (继续下一轮)
    │
    └─ StopEvent (结束)
```

### 多代理意图识别流程

```
┌──────────────────────────────────────────────────────────┐
│ 用户请求: "帮我计算和翻译"                                │
└──────────────┬───────────────────────────────────────────┘
               │
               v
    ┌─────────────────────────────┐
    │ Root Agent (多代理路由器)    │
    │ description: "协调所有请求" │
    └─────────────────────────────┘
               │
               v
    ┌─────────────────────────────────┐
    │ LLM收到handoff_prompt:           │
    │                                 │
    │ 当前可用代理:                    │
    │ - Calculator: 执行计算          │
    │ - Translator: 翻译文本          │
    └────────────┬────────────────────┘
                 │
                 v
    ┌─────────────────────────────────┐
    │ LLM判断: 需要先计算再翻译       │
    │ 调用: handoff(                  │
    │   to_agent="Calculator",        │
    │   reason="先完成数学计算"       │
    │ )                               │
    └────────────┬────────────────────┘
                 │
                 v
    ┌─────────────────────────────────┐
    │ aggregate_tool_results()        │
    │ 读取 next_agent = "Calculator"  │
    │ 切换: current_agent = Calculator│
    └────────────┬────────────────────┘
                 │
                 v
    ┌─────────────────────────────────┐
    │ Calculator Agent 接手           │
    │ description: "执行计算"         │
    │ tools: [add, subtract, ...]     │
    │                                 │
    │ 执行计算 → 返回结果             │
    └────────────┬────────────────────┘
                 │
                 v
    ┌─────────────────────────────────┐
    │ 计算完成, 需要翻译结果         │
    │ 再次调用: handoff(              │
    │   to_agent="Translator",        │
    │   reason="翻译计算结果"         │
    │ )                               │
    └────────────┬────────────────────┘
                 │
                 v
    ┌─────────────────────────────────┐
    │ Translator Agent 接手           │
    │ description: "翻译文本"         │
    │ tools: [translate_api, ...]     │
    │                                 │
    │ 翻译结果 → 返回最终输出         │
    └────────────┬────────────────────┘
                 │
                 v
    ┌─────────────────────────────────┐
    │ 返回 StopEvent                  │
    │ 最终结果已生成                  │
    └─────────────────────────────────┘
```

## 工作流步骤对应关系

### BaseWorkflowAgent 的6个工作流步骤

```python
# step 1
init_run(Context, AgentWorkflowStartEvent) -> AgentInput
  初始化内存、状态、消息

# step 2
setup_agent(Context, AgentInput) -> AgentSetup
  准备LLM输入 (加系统提示词和状态)

# step 3
run_agent_step(Context, AgentSetup) -> AgentOutput
  执行代理逻辑 (调用take_step())

# step 4
parse_agent_output(Context, AgentOutput) -> Union[StopEvent, AgentInput, ToolCall, None]
  判断是否需要工具调用或结束

# step 5 (条件执行)
call_tool(Context, ToolCall) -> ToolCallResult
  执行单个工具

# step 6 (条件执行)
aggregate_tool_results(Context, ToolCallResult) -> Union[AgentInput, StopEvent, None]
  汇总工具结果, 决定继续还是结束
```

### AgentWorkflow 的对应步骤

```python
# 完全相同的步骤名和签名, 但添加了多代理逻辑

# step 1: init_run
  初始化agents字典, can_handoff_to映射, root_agent

# step 2: setup_agent
  从agents[current_agent_name]获取当前代理配置

# step 3: run_agent_step
  调用agents[current_agent_name].take_step()
  动态添加handoff工具

# step 4: parse_agent_output
  使用当前代理的output_cls/structured_output_fn

# step 5: call_tool
  从agents[current_agent_name]获取工具

# step 6: aggregate_tool_results
  [关键]: 检查next_agent, 更新current_agent_name
  调用agents[current_agent_name].handle_tool_call_results()
```

## 核心概念讲解

### 1. Scratchpad (便签簿)
用于存储一个代理的中间状态 (多轮对话)

**FunctionAgent/CodeActAgent使用**:
```
scratchpad = [
  ChatMessage(role="user", content="计算1+1"),
  ChatMessage(role="assistant", tool_calls=[add(1,1)]),
  ChatMessage(role="tool", content="2"),
  ChatMessage(role="assistant", content="1+1=2")
]
```

### 2. Reasoning Chain (推理链)
用于存储ReActAgent的思维过程

**ReActAgent使用**:
```
current_reasoning = [
  ActionReasoningStep(thought="需要计算", action="add", action_input={"a": 1, "b": 1}),
  ObservationReasoningStep(observation="2"),
  ResponseReasoningStep(thought="得到答案", response="1+1=2")
]
```

### 3. Handoff Tool (切换工具)
允许一个代理调用另一个代理

**特殊性质**:
- return_direct=True (不继续迭代)
- 工具描述包含所有可用代理的信息
- 触发agent_name的更新

### 4. Context Store
保存运行时状态

**关键键值**:
```python
"memory"                          # ChatMemoryBuffer
"state"                           # 共享状态
"current_agent_name"              # 多代理中的当前代理
"next_agent"                      # 多代理中的下一个代理 (由handoff设置)
"current_tool_calls"              # 跟踪所有工具调用
"num_tool_calls"                  # 本轮工具调用数
"num_iterations"                  # 迭代计数
"max_iterations"                  # 最大迭代数
"{agent.scratchpad_key}"          # FunctionAgent/CodeActAgent的scratchpad
"{agent.reasoning_key}"           # ReActAgent的推理链
```

## 代码示例

### 示例1: 单代理工作流 (FunctionAgent)

```python
from llama_index.core.agent.workflow import FunctionAgent
from llama_index.core.tools import FunctionTool
from llama_index.core.llms import OpenAI

# 定义工具
def add(a: int, b: int) -> int:
    """Add two numbers."""
    return a + b

# 创建代理
agent = FunctionAgent(
    name="calculator",
    description="A calculator agent",
    system_prompt="You are a helpful calculator assistant.",
    tools=[FunctionTool.from_defaults(fn=add)],
    llm=OpenAI(model="gpt-4")
)

# 运行代理
handler = agent.run(user_msg="What is 1 + 1?")
result = handler.result()
print(result.response.content)
```

### 示例2: 多代理工作流

```python
from llama_index.core.agent.workflow import AgentWorkflow, FunctionAgent
from llama_index.core.tools import FunctionTool

# 定义多个代理
calculator_agent = FunctionAgent(
    name="calculator",
    description="Performs arithmetic calculations",
    system_prompt="You are a calculator.",
    tools=[FunctionTool.from_defaults(fn=add)],
    llm=OpenAI(model="gpt-4")
)

translator_agent = FunctionAgent(
    name="translator",
    description="Translates text to English",
    system_prompt="You are a translator.",
    tools=[...],  # translation tools
    llm=OpenAI(model="gpt-4")
)

# 创建多代理工作流
workflow = AgentWorkflow(
    agents=[calculator_agent, translator_agent],
    root_agent="calculator",  # 从calculator开始
    handoff_prompt="If the task requires translation, hand off to the translator.",
)

# 运行工作流
handler = workflow.run(
    user_msg="Calculate 1+1 and translate the result to French"
)
result = handler.result()
print(result.response.content)
```

### 示例3: ReActAgent (推理链)

```python
from llama_index.core.agent.workflow import ReActAgent

agent = ReActAgent(
    name="reasoner",
    description="An agent that reasons through problems",
    tools=[FunctionTool.from_defaults(fn=add)],
    llm=OpenAI(model="gpt-4")
)

handler = agent.run(user_msg="Why is 1+1=2?")

# 获取推理过程
for event in handler.event_stream():
    if hasattr(event, 'raw'):
        print(f"Agent thinking: {event.raw}")
```

### 示例4: 结构化输出

```python
from pydantic import BaseModel
from typing import Optional

class CalculationResult(BaseModel):
    question: str
    answer: int
    explanation: str

# 方式1: 使用output_cls
agent = FunctionAgent(
    name="calculator",
    tools=[FunctionTool.from_defaults(fn=add)],
    output_cls=CalculationResult,
    llm=OpenAI(model="gpt-4")
)

handler = agent.run(user_msg="What is 1+1?")
result = handler.result()
print(result.structured_response)  # CalculationResult instance

# 方式2: 使用structured_output_fn
def extract_result(messages: list) -> dict:
    # 自定义逻辑提取结构化结果
    return {
        "question": "What is 1+1?",
        "answer": 2,
        "explanation": "..."
    }

agent = FunctionAgent(
    name="calculator",
    tools=[FunctionTool.from_defaults(fn=add)],
    structured_output_fn=extract_result,
    llm=OpenAI(model="gpt-4")
)
```

### 示例5: CodeActAgent (代码执行)

```python
from llama_index.core.agent.workflow import CodeActAgent

async def code_execute_fn(code: str) -> dict:
    """Execute Python code safely."""
    # 实现安全的代码执行
    local_vars = {}
    try:
        exec(code, {}, local_vars)
        return {"success": True, "output": str(local_vars)}
    except Exception as e:
        return {"success": False, "error": str(e)}

agent = CodeActAgent(
    name="code_agent",
    description="An agent that can write and execute code",
    code_execute_fn=code_execute_fn,
    tools=[FunctionTool.from_defaults(fn=add)],
    llm=OpenAI(model="gpt-4")
)

handler = agent.run(user_msg="Calculate 1+1 using Python")
```

## 常见模式

### 模式1: 顺序处理 (多代理链)

```
Agent1 → Agent2 → Agent3 → Result

流程: 每个代理完成后调用handoff切换到下一个
```

### 模式2: 条件分支

```
            ┌─> Agent_Path_A ─┐
Request ────┤                 ├─> Final Result
            └─> Agent_Path_B ─┘

流程: Root Agent判断后选择合适的路径
```

### 模式3: 并行处理 (未直接支持)

```
需要自定义: 一个代理生成多个工具调用, 在handle_tool_call_results中并行处理
```

### 模式4: 递归 (受max_iterations限制)

```
Agent处理 → 需要更多信息 → handoff到信息收集Agent → 返回结果 → 继续处理

注意: max_iterations限制了总迭代次数
```

## 状态管理最佳实践

1. **共享状态vs私有状态**:
   - 共享: initial_state (所有代理可见)
   - 私有: scratchpad, reasoning_key (单代理内部)

2. **内存管理**:
   - 每个工作流有一个ChatMemoryBuffer
   - 所有代理共享该内存
   - finalize()负责将中间步骤添加到内存

3. **上下文更新**:
   ```python
   # 在工作流步骤中更新状态
   state = await ctx.store.get("state")
   state["key"] = new_value
   await ctx.store.set("state", state)
   ```

## 性能考虑

1. **迭代次数**: max_iterations限制 (默认20)
   - 防止无限循环
   - 每次handoff计数

2. **工具调用**: 
   - FunctionAgent支持并行tool_calls
   - ReActAgent/CodeActAgent单工具调用

3. **内存占用**:
   - ChatMemoryBuffer持续增长
   - 可配置max_tokens限制

4. **流式输出**:
   - 设置streaming=True可以提高响应体验
   - AgentStream事件提供增量更新

