
## LLM 的 http stream 

一、Web/API 语境里“流式传输/流式响应”的常见实现方式与提出时间

  

1）轮询 Polling

最早的思路就是客户端按固定间隔发 HTTP 请求问“有新数据吗”。这是 Web 早期就存在的做法，没有一个单独的标准“提出时间”，本质上跟 HTTP 请求-响应模型同龄。

  

2）长轮询 Long Polling（Comet 家族的一种）

把一次请求在服务端“挂起”，直到有数据或超时再返回，客户端立刻再发下一次请求。Comet 作为对这类技术的统称，术语在 2006 年由 Alex Russell 命名并流行起来。 

  

3）HTTP Streaming（基于持久连接 + 分块/增量写出）

在同一个 HTTP 响应里持续写入内容（常见于 ASGI/WSGI 的流式响应）。HTTP/1.1 的 chunked transfer-coding 在 RFC 2616（1999 年 6 月）里被标准化并广泛使用。 

  

4）SSE（Server-Sent Events / EventSource）

浏览器原生的一种“单向推送”：客户端用 EventSource 建立 HTTP 连接，服务端用 text/event-stream 持续推送事件。W3C 的工作草案在 2009 年就出现，2012 年进入关键草案阶段（Last Call WD），并在 2015 年成为 W3C Recommendation；其早期机制在 WHATWG 体系里可追溯到 2004 年。 

  

5）WebSocket

一次 HTTP Upgrade 握手后升级为全双工长连接，适合双向实时交互。IETF 在 RFC 6455（2011 年 12 月）标准化。 

  

6）HTTP/2（多路复用“流”）

HTTP/2（RFC 7540，2015 年 5 月）提供单连接多路复用等能力；它的“stream”更多是传输层语义，不等同于应用层 SSE/WebSocket，但能显著改善并发与延迟表现。 

  

7）gRPC Streaming

基于 HTTP/2 的 RPC 框架，天然支持 server-streaming / client-streaming / bidi-streaming。gRPC 在 2015 年 3 月对外宣布并开源，2016 年 8 月宣布 1.0/GA。 

  

8）HTTP/3 / QUIC（更现代的传输底座）

QUIC 的核心规范 RFC 9000 在 2021 年 5 月发布，HTTP/3 的 RFC 9114 在 2022 年 6 月发布；它们同样是传输与协议映射层能力，应用层仍需选择 SSE/WebSocket/gRPC 等模式。 

  

二、Python（FastAPI）里实现“流式响应”的典型方式

  

1）StreamingResponse（HTTP 流式写出）

FastAPI 基于 Starlette，Starlette 的 StreamingResponse 支持普通生成器或异步生成器，边产出边发送。 

  

2）SSE（text/event-stream）

本质还是 StreamingResponse，但内容要按 SSE 帧格式输出（每条消息以 data: 行 + 空行分隔）。

  

3）WebSocket

FastAPI 提供 WebSocket 路由，适合双向对话、需要客户端持续发送消息的场景。

  

三、Python LlamaIndex + FastAPI 实现流式传输的常见做法

  

思路是：让 LlamaIndex 产出 token（或文本片段）的生成器，再把它包装成 FastAPI 的 StreamingResponse（或 SSE/WebSocket）。

  

1）Query Engine（典型 RAG 查询）

LlamaIndex 的 Query Engine 可以用 streaming=True 打开流式；返回对象提供 response_gen 迭代器，你可以逐 token 读取。官方也提示：如果一次查询内部有多次 LLM 调用，通常只会从“最后一次 LLM 调用”开始流式输出。 

  

最小示例（纯文本流，适合 curl 或 fetch 流式读取）：

```python
from fastapi import FastAPI
from fastapi.responses import StreamingResponse

# 伪代码：你需要自己初始化 index / query_engine
# query_engine = index.as_query_engine(streaming=True)

app = FastAPI()

@app.get("/rag/stream")
def rag_stream(q: str):
    streaming_response = query_engine.query(q)  # streaming=True 时返回可流式对象

    def gen():
        for token in streaming_response.response_gen:
            yield token  # str 或 bytes 都可
    return StreamingResponse(gen(), media_type="text/plain; charset=utf-8")
```

SSE 版本（浏览器 EventSource 友好）：

```python
import json
from fastapi import FastAPI
from fastapi.responses import StreamingResponse

app = FastAPI()

@app.get("/rag/sse")
def rag_sse(q: str):
    streaming_response = query_engine.query(q)

    def event_gen():
        for token in streaming_response.response_gen:
            payload = json.dumps({"token": token}, ensure_ascii=False)
            yield f"data: {payload}\n\n"
        yield "event: done\ndata: [DONE]\n\n"

    headers = {
        "Cache-Control": "no-cache",
        "Connection": "keep-alive",
        # 如果前面有 Nginx，通常还需要关缓冲：X-Accel-Buffering: no
    }
    return StreamingResponse(event_gen(), media_type="text/event-stream; charset=utf-8", headers=headers)
```

2）Chat Engine（多轮对话）

Chat Engine 的流式接口是 stream_chat（异步是 astream_chat），并且同样可以通过 response_gen 逐 token 迭代。 

  

示例（SSE，适合前端聊天逐字显示）：

```python
import json
from fastapi import FastAPI
from fastapi.responses import StreamingResponse

# chat_engine = index.as_chat_engine()  # 文档强调：流式用 stream_chat，而不是 chat  [oai_citation:10‡LlamaIndex](https://developers.llamaindex.ai/python/framework/module_guides/deploying/chat_engines/usage_pattern/)

app = FastAPI()

@app.get("/chat/sse")
def chat_sse(msg: str):
    stream = chat_engine.stream_chat(msg)

    def event_gen():
        for token in stream.response_gen:
            yield f"data: {json.dumps({'token': token}, ensure_ascii=False)}\n\n"
        yield "event: done\ndata: [DONE]\n\n"

    return StreamingResponse(event_gen(), media_type="text/event-stream; charset=utf-8")
```

补充建议（实战里很关键）

如果你发现“服务端在 yield，但前端还是等到最后才显示”，常见原因是中间层缓冲（反向代理、网关、浏览器策略）。这时优先选 SSE（浏览器对 text/event-stream 更愿意逐段交付），并在反向代理层关闭缓冲。

  

如果你说的“流传输”指的是音视频（HLS/DASH/WebRTC 等）而不是 LLM token 流，也可以告诉我你的业务场景，我再按媒体流那一套协议体系把实现方式和时间线补齐。

### yield

`yield` 是 Python 生成器（generator）的关键字，用来”逐个产出”值，而不是一次性返回所有结果。

#### 对比 return 和 yield

```python
# 普通函数：一次性返回全部
def get_all():
    return ["a", "b", "c"]

result = get_all()  # 立即得到完整列表 ["a", "b", "c"]

# 生成器函数：逐个产出
def get_one_by_one():
    yield "a"
    yield "b"
    yield "c"

gen = get_one_by_one()  # 返回生成器对象，还没执行
for item in gen:        # 每次循环才执行到下一个 yield
    print(item)
```

#### 在流式传输中的作用

```python
async def generate():
    response = query_engine.query(q)
    for text in response.response_gen:
        yield text  # 每产生一段文字，立即发送给客户端
```

执行流程：

1. LlamaIndex 生成第一段文字 → `yield` 立即发出 → 客户端收到
1. 生成第二段 → `yield` 发出 → 客户端收到
1. 依此类推…

**不用 yield 的话**：

```python
def generate():
    result = ""
    for text in response.response_gen:
        result += text
    return result  # 等全部生成完才返回，用户要等很久
```

#### 简单理解

|    |return     |yield     |
|----|-----------|----------|
|比喻  |写完整本书再交给读者 |写一页给一页    |
|内存  |全部加载       |按需产出      |
|适合场景|数据量小、需要完整结果|流式输出、大数据处理|

所以 `yield text` 的意思就是”把这段 text 先发出去，函数暂停在这里，下次继续”。这正是实现流式响应的核心机制。​​​​​​​​​​​​​​​​



## 大文件如音视频 的 http stream


1. 两类“流”的核心差异

LLM 流（token streaming）
	•	目标：尽快看到模型生成的后续文字
	•	典型实现：一个 HTTP 响应持续写出（chunked transfer）或 SSE（text/event-stream）
	•	客户端需求：顺序接收即可，不需要随机跳转、也不需要严格时间戳

音视频流（media streaming）
	•	目标：边下边播，且能拖动进度、切清晰度、抗抖动、低延迟直播等
	•	客户端需求很硬：
	•	随机访问：播放器经常需要跳到某个时间点，对应“取某一段字节/某一段分片”
	•	码率自适应：网络变差就切低码率、变好就切高码率
	•	解码结构：GOP/关键帧决定能否从某处无缝开始解码
	•	结论：音视频更多是“很多个 HTTP 请求组成的流”（manifest + segments），而不是“一个请求一直传”。

2. 音视频场景下，HTTP 传输的三种主流形态

A) Progressive Download（单文件渐进式下载）

就是给一个 MP4/MP3 文件 URL，浏览器或播放器边下边播。要点是“Range 请求”：
	•	客户端会发：Range: bytes=... 请求某一段字节
	•	服务端要回：206 Partial Content + Content-Range，并声明 Accept-Ranges（支持范围请求）  

为什么播放器爱 Range：
	•	MP4 需要先读头部元数据才能开始（常见的“fast start”把关键元数据放前面），播放器会先拉一小段头，再按需拉后续字节
	•	拖动进度条本质上就是请求另一个字节范围

这类方式适合：
	•	点播 VOD、文件不太大、对 ABR 不敏感
不太适合：
	•	直播、需要强自适应、需要很低延迟

B) 真“一个响应不停写”的 HTTP Streaming（chunked）

HTTP/1.1 的 chunked transfer-coding 允许服务端不知道总长度也能分块持续发送数据（Transfer-Encoding: chunked）。  

它在音视频里的典型用途：
	•	传统网络电台：持续推送 MP3/AAC 裸流（很多客户端能播）
	•	但对现代视频播放（尤其浏览器 <video> + 各种播放器栈）往往不够友好，因为：
	•	播放器常常依赖 Content-Length、Range、分片边界、关键帧对齐、缓存策略
	•	中间层（反向代理/CDN）可能缓冲，导致你“写了但用户收不到”，延迟暴涨

所以：chunked 在音视频里能用，但更多是“特定播放链路/特定格式”的方案，不是最通用的互联网视频分发方案。

C) 分片式 HTTP 流媒体（HLS / DASH 这类）

这是今天互联网最常见的“HTTP 流媒体”含义：把媒体切成很多小段（segments / parts），客户端先拉一个清单（manifest/playlist），再按时间顺序拉分片播放，并可按网络状况切码率。

以 HLS 为例：RFC 8216 定义了用 playlist + media segments 传“无界的多媒体流”。  
它的好处：
	•	CDN 天然友好：每个分片是普通 HTTP 静态对象，易缓存
	•	ABR 简单：清单里多码率版本，客户端选最合适的拉
	•	直播也行：清单会不断更新，客户端周期性刷新获取新分片（这点有点像“轮询”，但对象是 playlist）

低延迟版本（LL-HLS）进一步把一个 segment 切成更小的 partial segments（EXT-X-PART 等），并通过 preload hint 等机制降低端到端延迟；相关标签和机制在 IETF 的 HLS 更新草案里详细描述。  

3. “详解”到 HTTP 层：Range vs chunked vs 分片下载分别长什么样

Range（点播、拖动、断点续传的核心）

请求（示例）：
	•	GET /video.mp4
	•	Range: bytes=0-1048575

响应（核心语义）：
	•	206 Partial Content
	•	Content-Range: bytes 0-1048575/总长度
	•	Accept-Ranges: bytes  

播放器行为（常见模式）：
	•	先拉头部少量字节解析容器信息
	•	再按播放进度持续发 Range 拉后续
	•	拖动时直接发新的 Range

服务端实现要点：
	•	正确处理 206 / 416（越界）
	•	单 Range 和多 Range（有的客户端会要多段）
	•	配合缓存（ETag/Last-Modified）让 CDN/浏览器高效复用

chunked（一个响应持续写）

响应头关键点：
	•	Transfer-Encoding: chunked  

传输体：
	•	每块前面有十六进制长度，最后用 0 长度块结束（理论上直播可以“不结束”，直到断开）

工程要点：
	•	中间层缓冲是最大敌人：反代/CDN/应用服务器的 flush 策略会直接决定你是否真的“流”
	•	浏览器视频栈不一定把这种无界 chunked 当成可 seek 的媒体源

HLS/DASH 分片下载（更像“很多个普通 HTTP GET”）
	•	playlist/manifest：小文本文件，直播场景会频繁刷新，通常要禁缓存或非常短缓存
	•	segments/parts：二进制媒体分片，适合被 CDN 缓存（点播尤其明显）
	•	客户端：拉清单 → 拉分片 → 缓冲到阈值开始播 → 后台继续拉后续分片 → 必要时切码率

这就是为什么它被叫“HTTP streaming”：整体观看体验是流式的，但传输是分片的。

4. 你在 FastAPI 里做音视频 HTTP“流”的落地建议
	•	如果是“给用户点播一个 MP4 文件”：优先把 Range/206 支持做好（否则某些浏览器/播放器会报错或无法拖动）。Range 语义在 HTTP 里是标准能力。  
	•	如果是“直播、要 ABR、要 CDN、要跨平台”：优先做 HLS（或 DASH）分片，FastAPI 可以当源站提供 playlist + segments，但生产环境往往用专门的打包器/边缘分发来做。
	•	如果你确实要“一个 HTTP 响应持续推媒体字节”：那更接近网络电台/特殊播放器链路，能用 chunked，但要严格评估播放器兼容性与中间层缓冲问题。  
