
可以，按你这个仓库的形态，我建议把这件事做成一套**“内部封装的频域能力”**，而不是把 kissfft 直接摊到对外 ABI 上。

你的仓库现在已经有几个很适合接振荡检测的基础条件：  
一是 `alg_run / alg_destroy` 这样的稳定 C ABI；二是实例化状态模型；三是 Java / Python 示例；四是已有 Python 仿真与离线画图链路。对这种结构，最稳的做法不是“让外部直接知道 kissfft”，而是把 kissfft 藏在 `algo_base` 内部，外部只看到你的输入点位、输出点位和检测结果。这样 ABI 边界像船壳，FFT 像机舱，换零件不影响乘客。

## 先给结论

如果你现在要把 kissfft 接进这个仓库，我推荐的主路线是：

**路线 A，最推荐**  
把 kissfft 作为 `third_party` 源码依赖纳入仓库，编译成你自己的 `libalgo_base.so` 的一部分，**静态并入**你的动态库，不额外暴露 `libkissfft*.so`。

原因很直接：

1. 你当前对外发布物核心是 `libalgo_base.so`，再引入一个额外运行时共享库，会让 Python `ctypes`、Java JNA/Panama、Docker 部署都多一层装载与路径管理。
2. kissfft 的头文件和库之间存在**编译宏必须一致**的问题。官方 FAQ 明确提到，常见错误之一就是“混合构建环境导致 `FIXED_POINT` / `kiss_fft_scalar` 定义不一致”；社区打包讨论里也专门提到，下游如果只拿头文件和库，还得复制这些 defines 逻辑。把 kissfft 封在你自己内部，最省心。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))
3. kissfft 本身就主打“简单可嵌入”，它支持 Make 和 CMake，默认 float，也可以切到 double / int16_t / int32_t / simd，并支持静态或共享库构建。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))


---

## 一、整体规划，不碰算法细节，只说架构

### 1. 对外 ABI 层保持稳定

你现在的 `alg_run(const AlgoIn*, AlgoOut*)` / `alg_destroy(const char*)` 不动。  
振荡检测能力作为**算法内部增强**接进去，而不是再暴露一个 “kissfft_run” 之类的新接口。

这样做有两个好处：
- Java / Python 示例可以继续沿用现有 ABI；
- Python 仿真系统、CSV 回放、JSONL 日志、plot 脚本也都能继续复用，只需要在 manifest 和输出点位里增加新的观测量。


### 2. 逻辑分层建议

结合你仓库现在的 `exports / manager / base / algorithm` 四层，我建议这样挂：

- **exports 层**  
    继续只做 ABI 解析与点位映射。

- **manager 层**  
    按 `instance_id` 保存每个实例的 FFT 状态、滑动窗口、ring buffer、配置、上次检测状态。

- **base 层**  
    放通用频谱处理基础件，比如 window buffer、预处理、频谱结果结构。

- **algorithm 层**  
    放 `OscillationDetector`，它只依赖 base 层和一个很薄的 `kissfft_adapter`。


### 3. 运行时调用链

建议你的内部流程长这样：

`alg_run`  
→ 解析输入点位  
→ 找到/创建实例状态  
→ 将最新采样推进滑动窗口  
→ 数据不足时返回 “未 ready”  
→ 数据足够时做预处理  
→ 调用 kissfft 做 real FFT  
→ 计算频域指标  
→ 与状态机 / 阈值 / 迟滞结合  
→ 输出振荡标志、主频、能量、置信度之类的结果  
→ `alg_destroy` 时释放实例资源

注意，这里我没有给振荡判据本身下配方，只是把机床摆好。

---

## 二、为什么在你的仓库里优先用 `kiss_fftr`，而不是 `kiss_fft`

如果输入信号是实数序列，优先用 **real FFT** 接口，也就是 `kiss_fftr`。  
官方说明里明确说，real-optimized FFT 返回的是**正频半谱**，输出长度是 `nfft/2 + 1` 个复数 bin；并且这个接口要求 `nfft` 必须是**偶数**。官方还提到，real 优化版本相比把实数硬塞进 complex FFT，能省下大约 45% CPU。([GitHub](https://github.com/mborgerding/kissfft/blob/master/README.md "kissfft/README.md at master · mborgerding/kissfft · GitHub"))

对振荡检测这类“实数时域信号 → 频谱峰值/能量分布”的场景，这个接口最顺手。

所以内部 API 规划建议是：

- 对外仍然是你的 `AlgoIn / AlgoOut`

- 对内 FFT wrapper 统一只暴露：
    - `init(nfft, datatype, ...)`
    - `execute_real_forward(samples)`
    - `get_spectrum()`
    - `destroy()`

---

## 三、目录与文件规划

建议新增一个依赖区和一个适配层：

```text
third_party/
  kissfft/

src/
  base/
    spectrum_types.h
    window_buffer.h
    window_buffer.cpp
    fft_preprocess.h
    fft_preprocess.cpp

  algorithm/
    oscillation_detector.h
    oscillation_detector.cpp

  manager/
    algo_instance_state.h
    algo_instance_state.cpp

  internal/
    kissfft_adapter.h
    kissfft_adapter.cpp
```

### 关键原则

**不要在公共头文件里 include `kiss_fft.h` / `kiss_fftr.h`。**

也就是：

- `src/rcny_alg_exports.h` 不要碰 kissfft
- Java / Python 示例不要碰 kissfft
- 只有 `kissfft_adapter.cpp` 这一小块知道 kissfft 的存在

这样可以把 kissfft 的类型宏、链接方式、头文件差异全部锁在内部，避免 ABI 污染。

---

## 四、依赖接入方式，有三种，你该选哪一种

### 方案 A：源码并入你的库，最推荐

把 kissfft 当作 vendored dependency，直接纳入仓库，比如：

```bash
git submodule add https://github.com/mborgerding/kissfft third_party/kissfft
```

或者不用 submodule，直接 vendor 固定版本源码。

**优点**
- 最少运行时依赖
- 不需要额外安装 `libkissfft.so`
- 最适合你现在的 `docker compose run --rm host-build` 这种一次性构建流程
- Java/Python 调用仍然只关心 `libalgo_base.so`

**缺点**
- 你自己负责依赖升级
- Makefile 要多编译几个 C 源文件

### 方案 B：先单独构建 kissfft 静态库，再链接进 algo_base

这也可以，而且很干净。  
官方 CMake 支持静态库和安装，且会导出 CMake package；输出库名会带 datatype，例如 `kissfft-float` 或 `kissfft-double`，并提供 `kissfft::kissfft` / `kissfft::kissfft-<datatype>` target alias。([GitHub](https://raw.githubusercontent.com/mborgerding/kissfft/master/CMakeLists.txt "raw.githubusercontent.com"))

**优点**
- 依赖管理更规整
- 以后若改成 CMake 主工程会更顺

**缺点**
- 你现在主工程是 Make + docker-compose，不一定比方案 A 更省事

### 方案 C：发布时同时携带 `libkissfft*.so`

不推荐。  
因为你最终交付会变成：

- `libalgo_base.so`
- `libkissfft-double.so` 或 `libkissfft-float.so`

这会把 Python/Java/Docker 部署链都变复杂，还要考虑 `LD_LIBRARY_PATH`、rpath、容器镜像内容、宿主机找库路径等问题。

---

## 五、版本选择建议

不要直接追 `master`。  
建议固定到一个**明确 tag 或 commit**。官方 release 页面在 2025 年已经引入 semantic versioning，能看到 `131.1.0`，以及后续的 `131.2.0` 相关更新。([GitHub](https://github.com/mborgerding/kissfft/releases "Releases · mborgerding/kissfft · GitHub"))

建议你这样做：

- 在仓库里记录 `third_party/kissfft/VERSION.txt`
- README 或 docs 里记下来源仓库、tag、commit
- 以后升级依赖时只做一次受控升级

一句话：不要让 FFT 依赖像野猫一样半夜自己长大。

---

## 六、构建策略怎么选 datatype

你的输入点位目前大量是 `ap.f64`，也就是 double 风格。  
从工程一致性角度，初版建议：

### 初版建议：`double`

构建 kissfft 时选 `KISSFFT_DATATYPE=double`。  
这样内部不需要来回把 `double -> float -> double` 转换，调试和结果解释会更干净。官方支持 `double`、`float`、`int16_t`、`int32_t`、`simd`。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))

### 后续性能版：`float`

如果后面确认瓶颈明显，再切到 `float`，内部做一次显式转换即可。

### 不建议初版就上 fixed-point 或 simd

- `int16_t / int32_t` 会把幅值缩放和解释复杂化
- `simd` 依赖 SSE，跨平台和 Docker 目标环境上更容易多出边角问题  
    官方也说明 `simd` 需要 SSE 支持。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))

---

## 七、最推荐的编译接法：把 kissfft 源码直接编进 `libalgo_base.so`

如果你保持现在的 Makefile 主导构建，那么最实用的路线通常是：

### 1. 只编你需要的最小源码集

对“实数信号前向 FFT”场景，通常至少需要：

- `kiss_fft.c`
- `kiss_fftr.c`

如果你后面要用缓存帮助器，再考虑 `kfc.c`。  
官方完整库默认会一起编更多文件，但你不一定都需要。官方 README 也指出复杂 FFT 在 `kiss_fft.c`，real FFT 在 `kiss_fftr.h` / 对应实现里。([GitHub](https://github.com/mborgerding/kissfft/blob/master/README.md "kissfft/README.md at master · mborgerding/kissfft · GitHub"))

### 2. 这些 `.c` 文件要按 C 编译

很重要。  
不要让它们被当成 C++ 文件编。你的主工程是 C++，但 kissfft 是 C 库。

### 3. 编译宏统一放在一个地方

如果你选 `double`，需要保证 kissfft 编译和引用它头文件的地方都看到一致配置。官方 FAQ 就点名了这个坑。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))

最稳妥方式是：

- `kissfft_adapter.cpp` 里 include kissfft 头
- 其他任何地方都不 include kissfft 头
- kissfft 的编译宏只在 Makefile 里设置一次

### 4. 链接进你的动态库

这样最终外部只看到：
```text
build/libalgo_base.so
```

而看不到单独的 kissfft 依赖。

---

## 八、如果你想把 kissfft 当外部静态库单独编出来，完整流程如下

这是“更规整但略重”的流程，适合你以后可能迁到 CMake 主工程的情况。

### 1. 获取源码

```bash
git submodule add https://github.com/mborgerding/kissfft third_party/kissfft
git submodule update --init --recursive
```

### 2. 在 Docker 构建容器里编译 kissfft

官方 README 说明它支持 CMake，最低 CMake 3.10，支持 `KISSFFT_DATATYPE`、`KISSFFT_STATIC`、`KISSFFT_OPENMP`、`KISSFFT_TEST`、`KISSFFT_TOOLS`、`CMAKE_INSTALL_PREFIX` 等选项。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))

你可以在 `host-build` 容器里执行：

```bash
cmake -S third_party/kissfft -B third_party/kissfft/build \
  -DKISSFFT_DATATYPE=double \
  -DKISSFFT_STATIC=ON \
  -DKISSFFT_TEST=OFF \
  -DKISSFFT_TOOLS=OFF \
  -DCMAKE_INSTALL_PREFIX=/opt/kissfft

cmake --build third_party/kissfft/build -j
cmake --install third_party/kissfft/build
```

### 3. 你的 Makefile 链接它

思路是：
```make
CPPFLAGS += -I/opt/kissfft/include/kissfft
LDFLAGS  += /opt/kissfft/lib/libkissfft-double.a
```

如果你使用 CMake package，也可以走 `find_package(kissfft CONFIG ...)`，因为官方 CMake 会安装 config 和 targets。([GitHub](https://raw.githubusercontent.com/mborgerding/kissfft/master/CMakeLists.txt "raw.githubusercontent.com"))

### 4. 仍然建议封装成内部 wrapper

即便 kissfft 已经单独安装，也别把它头文件散到整个工程里。  
还是让 `kissfft_adapter.cpp` 做唯一入口。

---

## 九、和你现有 Docker / Compose 流程怎么衔接

你现在仓库是通过 `docker-compose.yml` 的 `host-build` 服务在容器里执行 `make clean && make all`，产物回写到宿主机 `./build`。这对 kissfft 接入其实非常友好。

### 推荐改法

只需要把构建链扩成两段之一：

### 路线 A，最简单

`make all` 时顺手编译 `third_party/kissfft/kiss_fft.c` 和 `kiss_fftr.c`，再一起链接 `libalgo_base.so`。

这样用户还是执行原来的：

```bash
docker compose run --rm host-build
```

### 路线 B，规整版

在 `host-build` 里先 `cmake --build` 安装 kissfft 到固定路径，再跑你自己的 `make all`。

但从你当前工程风格看，我仍然建议路线 A。它像把发动机直接焊进车体，不用再拖一台小拖车。

---

## 十、内部调用过程，应该怎么组织

你说不要具体算法设计细节，那我就给你“调用骨架”。

### 每个实例维护这些状态

建议 manager 层保存：

- ring buffer / sliding window
- 当前 `nfft`
- `kiss_fftr_cfg`
- 输入临时 buffer
- 输出频谱 buffer
- 预处理参数
- 上次检测状态
- 是否已 ready

### 初始化时机

因为你已经有实例模型，建议：

- 第一次 `alg_run` 收到某个 `instance_id` 时懒初始化
- `alg_destroy(instance_id)` 时释放 `kiss_fftr_cfg` 和相关 buffer

### 单次调用节奏

1. 从 `AlgoIn` 取到本次样本
2. 推入窗口
3. 若窗口未满，输出 `ready=0`
4. 若窗口已满：
    - 做去均值 / 窗函数 / 可选去趋势
    - 调 `kiss_fftr(cfg, timedata, freqdata)`
    - 从 `freqdata[0 .. nfft/2]` 提取频谱特征
    - 更新检测状态
    - 输出结果

5. 返回 `rc`

### 为什么要缓存 cfg

官方用法是先 `kiss_fft_alloc` / `kiss_fftr_alloc` 生成 cfg，再重复调用 FFT，最后 free。它是典型“plan/config once, run many times”的模式。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))

所以不要每次 `alg_run` 都重新 alloc/free，那样会平白制造分配抖动。

---

## 十一、Makefile 级别你要关注的编译点

下面这些不是算法，而是“别踩坑说明书”。

### 1. 统一 include path

```make
CPPFLAGS += -I./third_party/kissfft
```

### 2. 让 C 文件用 C 编译器

```make
KISSFFT_C_SRCS := \
  third_party/kissfft/kiss_fft.c \
  third_party/kissfft/kiss_fftr.c
```

### 3. 如果选 double，统一宏

最保守做法是：

```make
CFLAGS   += -Dkiss_fft_scalar=double
CXXFLAGS += -Dkiss_fft_scalar=double
```

但更推荐你只让 `kissfft_adapter.cpp` 看见 kissfft 头，这样宏影响面最小。

### 4. 链接到你的 `.so`

把 kissfft 对象文件并入 `libalgo_base.so` 的链接输入。

### 5. 先别开 OpenMP

官方支持 OpenMP，但默认关闭。对你这种初版检测能力，不建议一上来就开。先把正确性、可复现性、部署复杂度控制住。([GitHub](https://github.com/mborgerding/kissfft "GitHub - mborgerding/kissfft: a Fast Fourier Transform (FFT) library that tries to Keep it Simple, Stupid · GitHub"))

---

## 十二、kissfft 运行时注意事项

### 1. `kiss_fftr` 的 `nfft` 必须是偶数

这是官方接口要求。([GitHub](https://raw.githubusercontent.com/mborgerding/kissfft/master/kiss_fftr.h "raw.githubusercontent.com"))

### 2. 浮点版本默认不做 scaling

官方说明里写得很清楚：

- floating-point 版本默认**不缩放**
- fixed-point 版本双向都会做缩放以防溢出 ([GitHub](https://github.com/mborgerding/kissfft/blob/master/README.md "kissfft/README.md at master · mborgerding/kissfft · GitHub"))

这会影响你后续对频谱幅值的解释。  
你虽然现在不需要算法细节，但工程上要知道：阈值设计不能拍脑袋假定“FFT 天生已经归一化”。

### 3. 核心例程线程安全

官方说 core routines 没有 static data，是 thread-safe 的。([GitHub](https://github.com/mborgerding/kissfft/blob/master/README.md "kissfft/README.md at master · mborgerding/kissfft · GitHub"))

这意味着你多实例并行是可以规划的，但前提仍然是每个实例自己的 buffer 和 cfg 不要乱共享。

### 4. 最好使用 out-of-place buffer

官方说明如果输入输出用同一块 buffer，它会创建临时 buffer。([GitHub](https://github.com/mborgerding/kissfft/blob/master/README.md "kissfft/README.md at master · mborgerding/kissfft · GitHub"))

所以内部最好明确分开：

- `time_buffer`
- `freq_buffer`

这样更可控。

---

## 十三、对你现有 manifest / simulator / test 的落地建议

这是你仓库一个很大的优势，应该狠狠干活。

### 1. manifest 扩点位，不改 ABI

你当前 Python 仿真系统是根据 `config/manifest.json` 动态生成 `AlgoIn / AlgoOut.points[]` 布局的，这特别适合给振荡检测加观测量。

建议未来输出里至少预留这类字段：

- `oscillation_detected`
- `dominant_frequency`
- `dominant_magnitude`
- `band_energy`
- `confidence`
- `spectrum_ready`

这样 Python 仿真和 Java/Python 示例都能自然跟上。

### 2. 先做离线回放验证

你的 simulator 已经支持：

- inline
- csv
- random
- JSONL 记录
- summary
- 离线 plot

这正好适合给振荡检测做“离线回放 + 可视化校验”。  
所以集成顺序建议是：

1. 先在 C++ 内部打通 FFT wrapper
2. 用 simulator 跑 CSV / synthetic 数据
3. 用 `plot_case_jsonl.py` 看结果
4. 稳定后再补 Java / Python demo 的说明

### 3. C++ 单测优先于跨语言联调

因为振荡检测一旦出问题，先要区分是：

- 算法逻辑问题
- FFT 调用问题
- 点位映射问题
- Python/Java 绑定问题

所以你应该先在 `test/` 里做：

- 固定输入窗口的频谱 sanity test
- 已知频率的主峰 test
- 数据不足窗口 test
- `alg_destroy` 后实例释放 test

---

## 十四、我对你这个仓库的推荐实施顺序

按性价比排序，我会这么落地：

### 第 1 阶段

把 kissfft vendor 进 `third_party/`，先固定版本。

### 第 2 阶段

新增 `kissfft_adapter`，只打通：

- init
- real FFT execute
- release

### 第 3 阶段

在 manager 层给每个实例加 window + cfg 缓存。

### 第 4 阶段

在 algorithm 层接入振荡检测判定框架，但先不折腾复杂规则。

### 第 5 阶段

扩 `manifest` 和 `AlgoOut`，让 simulator 能看见新输出。

### 第 6 阶段

用现有 CSV 回放和 plot 工具做离线验证。

### 第 7 阶段

补 README / docs，说明：

- 构建方式
- 新增输出点位
- 示例配置
- 结果解释

---

## 十五、最终推荐方案

- **kissfft 用 vendored dependency**
- **固定 tag/commit，不追 master**
- **选 `double` 作为初版 datatype**
- **只用 `kiss_fftr` 做实数前向 FFT**
- **不暴露 kissfft 到公共头文件**
- **把 kissfft 静态并入 `libalgo_base.so`**
- **每个实例缓存 cfg 和窗口**
- **先走 simulator 离线回放验证**
- **等功能稳定，再考虑 float / OpenMP / 更复杂频谱策略**
