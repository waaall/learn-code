
Portal 前端直连内网 API 在生产环境失败的根因与 Nginx 反向代理解决方案

---

## 0. 背景与问题定义

### 0.1 请求链路是谁在发请求？

在 Web 架构里，“谁发请求”决定了问题发生在哪一层：

- **浏览器发请求（Browser-side request）**  
    典型：`fetch/axios` 在用户电脑上执行。  
    **网络可达性、CORS、Mixed Content** 都由“用户浏览器所在网络 + 浏览器安全策略”决定。

- **服务器发请求（Server-side request）**  
    典型：Portal 后端（Node/Java）去请求内网 API。  
    这时浏览器不直接接触内网 IP，也就不会触发 CORS/Mixed Content 等浏览器侧限制。

你当前描述是**浏览器(用户电脑) → 按前端拼出来的 URL 发请求 → 后端**，并且拼 URL 发生在 `index.ts` / `apiClient.ts`，说明这是**浏览器直连**模型。

### 0.2 为什么开发时能通，生产不通？

因为你开发环境依赖的是 **Vite dev server 的 server.proxy**：

- 开发时：浏览器请求 `http://localhost:5173/api/...`  
    → **Vite 服务器代你转发**到 `http://192.168.50.50:5000/...`  
    → 浏览器看到的是同源（或至少是“本地开发源”），因此很多问题被“开发代理”遮蔽了。

- 生产时：没有 Vite dev server  
    → 浏览器真正去请求你拼出来的 URL（很可能是 `http://192.168.50.50:5000` 或者跨域/非 https）  
    → 于是**连通性 / CORS / Mixed Content / 网关转发规则缺失**全部暴露。
    

> 结论：Vite proxy 是“开发期便利设施”，不是生产架构的一部分。生产要靠**网关/反向代理**承接同样的职责。

---

## 一、问题分层

下面四类问题不是并列随机的，它们存在明确的“门槛顺序”：

> **连通性（L3/L4） → TLS/HTTPS（加密层） → 浏览器安全策略（Mixed Content / CORS） → 网关/反代路由（应用层）**

### 1. Web 侧连通性问题（最基础，L3/L4 网络层）

#### 1.1 含义

用户的浏览器发起 TCP 连接时，是否能到达目标：  
`192.168.50.50:5000`

这里关键是：**是用户电脑发起连接**，不是 Portal 服务器。  
所以必须满足：**每个访问者所在网络**都能路由到 `192.168.50.50/24` 这类 RFC1918 私网地址。

#### 1.2 常见现象与底层原因

- `ERR_CONNECTION_TIMED_OUT`  
    通常是**路由不可达**或中间防火墙丢包：SYN 发出去没人回（或被丢）。
- `ERR_CONNECTION_REFUSED`  
    TCP 能到主机，但目标端口没服务/被防火墙明确拒绝（RST）。
- 请求一直 pending  
    多见于网络层阻塞或 TLS 握手卡住（也可能是代理、DNS 等因素）。

#### 1.3 为什么“Portal 服务器能通”不等于“用户能通”

Portal 服务器可能在机房内网、同网段、有 VPN/专线；用户浏览器可能在公网/办公网另一侧。  
浏览器直连时，**Portal 服务器完全不参与转发**，它的连通性不影响浏览器。

#### 1.4 修复策略

- 让用户网络可达该内网 IP（同网段 / VPN / 零信任接入）
- **更推荐：不要让浏览器直连内网**，改为走网关反代（同域 `/api`），由网关去打内网。

---

### 2. CORS 跨域问题（浏览器同源策略，应用安全层）

#### 2.1 含义

只要 **协议/域名/端口**任一不同，就不是同源（Same-Origin）：

- 页面：`https://portal.xxx.com`
- API：`http://192.168.50.50:5000`

这就是跨域。

#### 2.2 底层原理：同源策略与 CORS

浏览器有一个核心安全机制：**同源策略（Same-Origin Policy）**  
目的：防止恶意网站在用户已登录/有凭据情况下，读取另一个站点的敏感数据。

CORS（Cross-Origin Resource Sharing）是“允许例外”的机制：  
服务器必须显式返回响应头，例如：

- `Access-Control-Allow-Origin: https://portal.xxx.com`（或更谨慎的白名单）
- `Access-Control-Allow-Credentials: true`（如果带 cookie/认证）
- 对预检请求（Preflight）要正确处理 OPTIONS：
    - `Access-Control-Allow-Methods`
    - `Access-Control-Allow-Headers`

#### 2.3 现象

- 控制台：`No 'Access-Control-Allow-Origin' header...`
- 或者：`CORS preflight channel did not succeed` / OPTIONS 失败

#### 2.4 修复策略（两条路）

- 后端完整实现 CORS（含 OPTIONS、允许的 headers/methods、credentials）
- **更推荐：同域化**：浏览器只请求 `https://portal.xxx.com/api/...`  
    由网关/反代转发到内网后端 → 浏览器视角不跨域 → 彻底消除 CORS 复杂度

---

### 3. HTTPS 混合内容（Mixed Content，浏览器更高优先级拦截）

#### 3.1 含义

HTTPS 页面调用 HTTP 资源：

- 页面：`https://portal.xxx.com`
- API：`http://192.168.50.50:5000`

这是“混合内容”。

#### 3.2 底层原理：为什么浏览器直接拦截

HTTPS 的安全模型要求：页面内的关键请求（XHR/fetch）必须同样安全，否则会被中间人攻击替换响应。  
所以浏览器会在“发请求之前/早期”就拦截，很多情况下你连 CORS 阶段都到不了。

#### 3.3 现象

- 控制台：`Mixed Content: The page at ... was loaded over HTTPS, but requested an insecure resource ... This request has been blocked`

#### 3.4 修复策略

- API 也上 HTTPS（证书、TLS 终止等）
- **更推荐：同域 HTTPS `/api`**，由网关对内转发到 HTTP（内网）  
    浏览器看到的是 HTTPS，同源安全；内网转发是否 HTTP 由基础设施决定。

---

### 4. 反向代理问题（生产最常见的“路由没配/配错”）

#### 4.1 含义

你把前端改成请求 `/api/...`，但生产网关/Nginx 没有把 `/api` 转到 QA 后端，于是：

- 请求落到 Portal 静态站点（返回 index.html 或 404）
- 或者 upstream 不通（502/504）
- 或者路径重写错误（后端收到的路径不对）

#### 4.2 现象与定位

- `/api/...` 返回 404：命中了静态站点或没有匹配 location
- 502 Bad Gateway：Nginx 到 upstream 连接失败（连通性/端口/防火墙）
- 504 Gateway Timeout：upstream 有但响应太慢/超时
- 返回 HTML（Portal 页面）：说明被前端路由兜底或静态资源处理吞掉了 API

#### 4.3 修复策略

在网关加规则，把：

- `/api/*`
- `/health`（或健康检查端点）

转发到：

- `http://192.168.50.50:5000`

并确保路径、Host、headers、超时策略合理。

---

## 二、为什么 Nginx 可以解决这些问题？

Nginx 能解决的不是“魔法”，而是它把“谁发请求”从浏览器换成了网关，并在不同层面消掉限制。

### 2.1 核心改变：浏览器不再直连内网

改造前：
- 浏览器 → `http://192.168.50.50:5000`（用户网络必须能到内网）

改造后：
- 浏览器 → `https://portal.xxx.com/api/...`（公网可达 portal 域名即可）
- Nginx（网关）→ `http://192.168.50.50:5000/...`（网关通常与后端同网段/有路由权限）

因此：
- **连通性问题**：从“每个用户都要能到内网”变成“只有网关要能到内网”
- **CORS 问题**：浏览器视角同域（`portal.xxx.com`），不跨域 → 不需要 CORS
- **Mixed Content**：浏览器只看到 HTTPS 请求 → 不再混合内容
- **生产代理缺失**：用 Nginx 在生产实现和 Vite proxy 同类能力

---

## 三、Nginx 反向代理的底层原理

### 3.1 Nginx 在请求链路中的角色

Nginx 是一个高性能的事件驱动网络服务器，常见用法包括：

- 静态资源服务
- 负载均衡（Load Balancing）
- **反向代理（Reverse Proxy）**
- TLS 终止（HTTPS 证书、握手、加解密在此完成）

反向代理的关键在于：  
**客户端（浏览器）只与 Nginx 建立连接**；Nginx 再作为客户端去连接上游（upstream）后端。

### 3.2 连接与转发在 TCP/HTTP 层怎么发生？

1. 浏览器与 Nginx 建立 TCP/TLS 连接（HTTPS）
2. 浏览器发送 HTTP 请求：`GET /api/users`
3. Nginx 匹配 `location /api/` 规则
4. Nginx 创建/复用到 upstream 的连接（可 keepalive）
5. Nginx 将请求转发给 upstream（可重写 URI、加 header）
6. upstream 返回响应
7. Nginx 将响应回传给浏览器

在这个过程中，浏览器永远不知道 `192.168.50.50:5000` 的存在。

### 3.3 为什么 Nginx 性能好（“能扛流量”）？

关键是它的架构特性：

- **事件驱动（event-driven）** + **非阻塞 IO（non-blocking I/O）**
- 少量 worker 进程就能处理大量并发连接
- 上游连接复用、缓冲、限速、超时控制等能力齐全

这使它非常适合做网关层的反向代理。

---

## 四、推荐的生产标准方案（同域 `/api` + Nginx 反代）

### 4.1 前端侧规范（重要）

目标：前端永远不要拼 `http://192.168...` 这种“环境内网地址”。

建议：
- `apiClient.ts` 统一使用相对路径：`/api` 作为 baseURL
- 通过环境变量控制“是否走代理”通常不是必要的；生产与开发都用 `/api` 最稳定
- 开发环境依旧可以在 Vite proxy 把 `/api` 转到 QA 后端，做到开发/生产一致

> 关键原则：**同一份前端代码，在开发与生产都请求同样的路径形态 `/api/...`。**

### 4.2 Nginx 配置示例（典型写法）

```nginx
server {
  listen 443 ssl;
  server_name portal.xxx.com;

  # 1) 静态站点
  location / {
    root /var/www/portal;
    try_files $uri $uri/ /index.html;
  }

  # 2) API 反向代理
  location /api/ {
    proxy_pass http://192.168.50.50:5000/;  # 注意末尾的 /
    proxy_http_version 1.1;

    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;

    # 可选：超时与上游容错
    proxy_connect_timeout 5s;
    proxy_read_timeout 60s;
  }

  # 3) 健康检查
  location = /health {
    proxy_pass http://192.168.50.50:5000/health;
  }
}
```

#### 关键点解释

- `location /api/` + `proxy_pass .../`（末尾 `/`）  
    决定了 URI 是否会被正确拼接/重写。这个细节经常导致“后端路径不对”。
- `try_files ... /index.html`  
    SPA 常用；但要避免 API 路由被它吞掉，所以 `/api/` 必须单独 location 且优先匹配。
- `X-Forwarded-*`  
    后端常用来识别真实协议/来源 IP。

---

## 五、诊断与验证清单

### 5.1 浏览器侧（用户现场）

- DevTools → Network 看请求 URL 是否仍然是 `192.168...:5000`
- Console 是否出现 Mixed Content / CORS 报错
- Network 是否有 OPTIONS（预检）失败
- 状态码是 404（路由）还是 502（上游连接）

### 5.2 网关侧（Nginx）

- access.log：是否命中 `/api/...`
- error.log：是否有 upstream connect failed / timeout
- 用网关机器 `curl http://192.168.50.50:5000/health` 验证上游连通性

### 5.3 后端侧

- 确认后端实际监听：`0.0.0.0:5000`（而不是 `127.0.0.1`）
- 防火墙/安全组是否允许来自网关的访问

---

## 六、结论与落地建议

1. **开发可用不代表生产可用**：Vite proxy 只在开发期生效，生产必须由网关/Nginx 实现等价能力。
2. 当前问题本质是：浏览器直连内网导致的多层失败（连通性 / Mixed Content / CORS）。
3. 最推荐架构：
    - 前端统一请求同域 `https://portal.xxx.com/api/...`
    - Nginx 负责 TLS 终止 + `/api` 反向代理到内网后端
    - 这样同时解决：连通性、CORS、Mixed Content，并使开发/生产一致。

---

