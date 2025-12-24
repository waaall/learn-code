

## 安装部署方式


### docker


```bash
docker pull nginx:stable
```
#### docker nginx 镜像版本选择

nginx 官方 docker 容器的版本名都是什么意思：stable-trixie-perl 、stable-trixie-otel、stable-trixie、stable-perl、stable-alpine-slim、stable？

#### 第一段：发布通道

stable 表示 NGINX 的稳定分支（版本号第二段为偶数，例如 1.28.x）。稳定分支通常只回补关键修复/安全修复，更新频率比 mainline 低。

#### 第二段：基础发行版（可选）

例如 trixie、alpine。像 trixie 这种名字是 Debian 各发行版的 suite 代号，用来标明镜像基于哪个 Debian 版本；如果你需要在镜像里额外 apt-get install 包，官方也建议显式指定这类代号，避免 Debian 基础镜像升级带来破坏性变化。

alpine 则表示基于 Alpine Linux，体积更小，但使用 musl libc，兼容性取舍不同。


#### 第三段：功能变体（可选）

perl、otel、slim 这类后缀表示在“同一发布通道 + 同一基础发行版”的前提下，额外加/减一些组件。

#### 版本名具体含义

1、stable-trixie-perl
稳定分支 + Debian trixie 基础 + 带 NGINX 的 Perl 模块。Perl 模块从默认镜像里被移除后，官方用 -perl 作为单独变体提供。

2、stable-trixie-otel
稳定分支 + Debian trixie 基础 + 带 OpenTelemetry 追踪模块（ngx_otel_module）。该模块提供分布式追踪，支持 W3C Trace Context 传播以及 OTLP/gRPC 导出（例如 otel_exporter 等指令）。
（在 Debian/NGINX 官方包体系里也能看到 nginx-module-otel 这类模块包的存在，包括 trixie 架构。 ）

3、stable-trixie
稳定分支 + Debian trixie 基础的“默认功能集合”（不额外加 perl/otel，也不是 slim）。

4、stable-perl
稳定分支 + Perl 模块，但不把 Debian suite 写死在 tag 里。也就是说它会跟随 stable 的默认 Debian 基础版本迁移（今天可能等同于 stable-trixie-perl，将来 Debian 默认基底换代时也会一起漂移）。关于“Debian suite 代号不写死会带来变动风险”的提醒，官方在变体说明里明确提过。

5、stable-alpine-slim
稳定分支 + Alpine 基础 + slim 精简变体。slim 的含义是去掉默认镜像里常见的一些包，只保留运行 nginx 所需的最小集合。


6、stable
稳定分支的“默认”镜像（不固定 Debian suite 代号）。它会随时间漂移到新的 Debian 基础版本；如果你追求可重复构建、并且要装额外系统包，通常更推荐用 `stable-<suite>`（例如 stable-trixie）把基础锁死。


