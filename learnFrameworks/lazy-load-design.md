
## 类型 A：模块级懒加载（Module Lazy Loading）

这是最基础、通常也最优先考虑的一类。

### 它解决什么

程序启动时不导入所有功能模块，而是在真正需要某个功能时再动态导入。

### 典型场景

- 插件系统
- 工具箱式桌面应用
- 任务中心 / 操作中心
- 命令分发系统
- 多页面 GUI

### 典型设计

核心思想如下：

- 启动时只保存轻量元数据
- 运行时根据 `module_path + class_name` 调用 `importlib.import_module`
- 第一次解析后缓存类对象

### 常见结构

```
@dataclass
class TaskDescriptor:
    key: str
    title: str
    module_path: str
    class_name: str
    settings_group: str
```

```
import importlib

class TaskLoader:
    _class_cache = {}

    @classmethod
    def load_class(cls, descriptor):
        cache_key = (descriptor.module_path, descriptor.class_name)
        if cache_key in cls._class_cache:
            return cls._class_cache[cache_key]

        module = importlib.import_module(descriptor.module_path)
        klass = getattr(module, descriptor.class_name)
        cls._class_cache[cache_key] = klass
        return klass
```

### 优点

- 对启动速度改善最直接
- 架构清晰
- 易于统一管理
- 适合向插件化演进

### 缺点

- 需要将“元数据”和“实现类”剥离
- 错误会从启动时推迟到运行时暴露
- 首次点击通常会有初始化等待

### 对当前项目的意义

当前项目的任务形态天然接近“任务列表 + 点击执行”，与 `registry/factory` 结构较为匹配。

需要补充说明的是，当前实现中后端对象在启动阶段已经完成实例化，设置页也直接依赖任务类及其类名做配置映射。因此，若采用模块级懒加载，仅将 `TaskDescriptor` 改为保存 `module_path` 和 `class_name` 仍然不够，还需要同步评估启动期实例化路径以及设置页映射逻辑，否则懒加载收益会被现有耦合部分抵消。

## 类型 B：对象级懒加载（Instance Lazy Loading）

这是第二层。

### 它解决什么

类已经完成导入，但对象暂不创建，直到真正执行任务时再实例化。

### 典型场景

- handler 较多，但单次运行通常只使用少数几个
- 对象构造会触发扫描、读配置或建立连接
- 页面较多，但首页不会全部使用

### 典型设计

```
class LazyTaskRunner:
    def __init__(self, descriptor, settings_provider):
        self.descriptor = descriptor
        self.settings_provider = settings_provider

    def run(self):
        cls = TaskLoader.load_class(self.descriptor)
        settings = self.settings_provider(self.descriptor.settings_group)
        instance = cls(settings)
        return instance.execute()
```

### 优点

- 改动通常比模块级懒加载更小
- 适合对象构造成本较高的情况
- 适合“注册很多入口，但实际只运行少数任务”的系统

### 缺点

- 如果模块顶层本身很重，它无法显著改善启动时间
- 容易让人误以为已经完成懒加载，实际上只完成了一部分

### 判断标准

如果启动慢点已经发生在：

```
from modules.ECG_handler import ECGHandler
```

这一类导入语句所在的阶段之前，那么对象级懒加载只能缓解实例构造成本，不能解决导入链过重问题。

### 采纳时需要注意

采用对象级懒加载时，应预先明确实例生命周期策略，例如：

- 每次运行新建实例
- 每个任务保留单例
- 按需缓存实例

不同策略会影响设置更新、信号连接、日志状态、资源释放以及线程使用方式。这些内容不属于本文的整改计划，但属于采纳该模式时需要提前识别的风险点。

## 类型 C：页面 / 功能级懒加载（Feature or View Lazy Loading）

这是 GUI 项目中非常常见的一类。

### 它解决什么

首页只渲染首屏必要内容，其他页面、面板、详情区、设置页、预览区按需构造。

### 典型场景

- 多标签页应用
- 左侧导航 + 右侧内容页
- 设置页结构复杂
- 预览页需要图表、图像或表格组件

### 典型设计

常见做法通常有两种。

#### 方案 1：首次访问时构造页面

```
class PageManager:
    def __init__(self):
        self._pages = {}

    def get_page(self, key):
        if key not in self._pages:
            self._pages[key] = self._build_page(key)
        return self._pages[key]
```

#### 方案 2：先显示骨架页，内容延迟加载

- 页面容器先创建
- 数据区先显示 loading / placeholder
- 真正内容异步填充

### 优点

- 对用户体感改善通常较明显
- 首屏加载更轻
- 可以与模块级懒加载叠加

### 缺点

- 页面首次切换通常会变慢
- 需要处理“首次进入中”的状态反馈
- 容易出现页面生命周期管理混乱

### 什么时候适合

当启动分析显示慢点在 UI 装配而非导入链时，这类优化的优先级就会上升。对于桌面 GUI，这通常属于首屏治理和非首屏延迟构造的常规做法。

## 类型 D：资源 / 依赖级懒加载（Heavy Dependency / Resource Lazy Loading）

这类方式经常被忽略，但通常非常关键。

### 它解决什么

即使已经做到模块按需导入，某个模块在加载后仍会立刻拉起以下重依赖或重资源：

- `matplotlib`
- `torch`
- `faster_whisper`
- `cv2`
- 大模型权重
- 字体缓存
- GPU context
- 大型配置扫描

这类情况下，首次进入功能时仍然可能出现明显停顿。

### 典型设计

将重依赖继续向更深层的实际使用位置下沉。

#### 不建议这样

```
# modules/ecg_handler.py
import matplotlib.pyplot as plt
import pandas as pd
import scipy
```

#### 更合理的方向

```
class ECGHandler:
    def execute(self):
        import matplotlib.pyplot as plt
        import pandas as pd
        import scipy
        ...
```

或者进一步拆成：

```
class ECGRuntime:
    _backend_ready = False

    @classmethod
    def ensure_backend(cls):
        if not cls._backend_ready:
            import matplotlib.pyplot as plt
            import pandas as pd
            import scipy
            cls._backend_ready = True
```

上述示例仅用于说明“导入下沉”的方向，并不直接构成完整的运行时封装。如果后续代码仍需持续使用 `plt`、`pd`、`scipy` 等对象，则需要显式保存模块引用，或在实际使用位置再次导入；仅执行一次局部 `import` 并不会自动形成可复用的运行时状态。

### 优点

- 对“少数超重依赖”通常非常有效
- 可以叠加在模块级懒加载之上
- 往往是启动优化的第二阶段主要收益来源

### 缺点

- 模块内部结构通常需要调整
- 依赖错误会推迟到运行期暴露
- 需要更明确的初始化状态提示

### 对当前项目的意义

对 RD-tools 而言，这一类价值较为明确。

`modules.ECG_handler` 顶层导入 `matplotlib/pandas/scipy`，`modules.gen_subtitles` 顶层导入 `faster_whisper/torch/transformers`，都属于典型的资源级懒加载候选。

## 类型 E：进程 / Worker 级懒加载（Process-Level Lazy Loading）

这是更重的一层隔离手段。

### 它解决什么

某些功能不仅导入成本高，而且会明显污染主进程，例如：

- 内存占用显著上涨
- `GPU / OpenMP / BLAS` 冲突
- 崩溃影响整个主应用
- 字体 / 动态库 / `ffmpeg` / `torch` 环境脆弱

此时，更合适的懒加载方式不是继续在主进程中导入，而是在真正使用时再拉起子进程。

### 典型场景

- AI 推理
- 视频处理
- 音频转写
- 大型科学计算
- 医学影像
- OCR / CV 管线

### 典型设计

#### 主进程

- 只管理 UI、任务列表和参数输入
- 点击执行后再启动 worker

#### Worker 进程

- 接收任务参数
- 再导入重依赖
- 执行后返回结果、日志或错误

### 结构示意

```
Main Process
 ├─ Task Registry
 ├─ UI
 └─ WorkerLauncher
        └─ subprocess / multiprocessing
                └─ import heavy libs
                └─ run task
```

### 优点

- 主进程可以保持轻量
- 崩溃隔离更清晰
- 内存治理通常更好
- 更适合极重任务

### 缺点

- 架构复杂度会上升
- IPC、日志、进度、错误处理都需要补齐
- 调试成本更高

### 什么时候适合

当某个任务已经不只是“启动稍慢”，而是同时具备以下问题时，应考虑这一层：

- 首次加载需要几秒到十几秒
- 内存和环境污染明显
- 失败可能拖垮整个桌面应用

此时就不应只将注意力放在 `importlib` 上。

# 业界常见的几种懒加载架构模式

上面是按问题类型分类，下面从架构模式角度进行归纳。

## 模式 1：Registry + Factory

这是当前项目较适合优先考虑的一种结构。

### 核心结构

- Registry 只注册元数据
- Factory 负责动态加载类并创建实例
- Runner 负责执行

### 架构图

```
TaskRegistry
  └─ TaskDescriptor(key, title, module_path, class_name, settings_group)

TaskFactory
  ├─ load_class(descriptor)
  └─ create_instance(descriptor, settings)

UI / Action Layer
  └─ on_click -> factory.create_instance(...) -> run()
```

### 适用

- 工具箱
- 批处理任务系统
- 插件式桌面应用
- 命令中心

### 优点

- 结构简单
- 稳定性较好
- 可维护性较高
- 后续容易演进成插件系统

## 模式 2：Manifest + Dynamic Import

这是 `Registry` 的进一步演化。

### 核心结构

将任务定义放到独立 manifest 中，例如 `JSON / YAML / Python dict`：

```
tasks:
  - key: ecg
    title: ECG 处理
    module_path: modules.ECG_handler
    class_name: ECGHandler
    settings_group: ECGHandler
```

### 好处

- 配置与代码实现进一步解耦
- 便于后续插件扩展
- 适合任务数量持续增长的场景

### 缺点

- 需要额外校验 manifest 的正确性
- 重构初期比 `Registry` 多一层抽象

### 什么时候用

当任务数量开始膨胀，或者未来计划开放给其他人扩展插件时，这种方式会更有价值。

## 模式 3：Lazy Proxy / Virtual Proxy

这是“代理壳对象”模式。

### 核心思路

先给 UI 一个行为上类似真实对象的代理对象，真正使用时再构造真实实例。

```
class LazyProxy:
    def __init__(self, factory):
        self._factory = factory
        self._target = None

    def _get_target(self):
        if self._target is None:
            self._target = self._factory()
        return self._target

    def __getattr__(self, item):
        return getattr(self._get_target(), item)
```

### 适用

- 希望兼容旧接口
- 现有系统大量假设“拿到的就是对象”
- 暂时不希望大改调用链

### 优点

- 对旧代码侵入较小
- 便于平滑迁移

### 缺点

- 隐式行为较强
- 调试时定位成本较高
- 错误位置可能更绕

### 建议

如果没有兼容性压力，通常不建议将其作为长期主架构。它更适合作为过渡方案。

## 模式 4：Dependency Injection + Lazy Provider

这类方式在大型后端系统中更常见。

### 核心思路

不是直接注册对象，而是注册 provider：

```
container.register("ecg_handler", lambda: ECGHandler(...))
```

第一次 `resolve` 时才真正创建实例。

### 适用

- 大型应用
- 已经存在服务容器
- 依赖关系较复杂

### 优点

- 依赖关系更可控
- 生命周期更容易管理
- 测试替换更方便

### 缺点

- 对当前项目而言通常偏重
- 容器引入成本较高
- 过早引入可能增加抽象复杂度

### 结论

对当前项目而言，现阶段通常没有必要引入完整的 DI 体系。

## 模式 5：Plugin System

这是更彻底的“功能岛屿”方案。

### 核心思路

每个功能都作为插件存在，并具备：

- 自己的 metadata
- 自己的 entry point
- 自己的生命周期
- 启用 / 禁用 / 扫描发现能力

### 典型设计

- 插件清单
- 插件发现器
- 插件加载器
- 插件接口规范

### 适用

- 大型桌面工具平台
- 多团队扩展
- 第三方插件生态

### 优点

- 扩展性极强
- 功能隔离自然
- 非常适合懒加载

### 缺点

- 复杂度高
- 设计成本大
- 不适合中小项目一步到位引入

## 模式 6：Worker Isolation

这就是前文所述进程级懒加载的工程化版本。

### 核心思路

UI 不直接接触重依赖，所有重任务通过 worker 执行。

### 适用

- AI / 多媒体 / 医疗影像 / 科学计算

### 优点

- 更稳定
- 隔离性更强
- 主应用体验通常更平滑

### 缺点

- 通信协议、状态同步、日志传递都需要设计

# 如何选择懒加载架构模式

可以按以下判断表做初步选择。

## 1. 启动后一打开就慢

优先考虑：

- 模块级懒加载
- 任务注册中心 + 工厂

说明：

问题通常位于入口导入链。对 RD-tools 的示例分析也表明，`import main` 明显偏慢，而 `main_window` 并不重，这属于典型的入口模块提前拉入重任务模块。

## 2. 启动不慢，但点击任务时会明显卡顿

优先考虑：

- 资源级懒加载
- 首次初始化提示

说明：

这通常意味着模块本身已经被延后加载，但模块内部仍在顶层拉起重依赖。

## 3. 首页正常，但切换页面较慢

优先考虑：

- 页面 / 功能级懒加载
- 首屏与非首屏拆分

## 4. 某些任务偶发崩溃、内存暴涨或依赖冲突

优先考虑：

- Worker 级懒加载
- 主进程与重任务隔离

## 5. 功能持续增多，维护复杂度持续上升

优先考虑：

- Manifest + Plugin 化
- 元数据与实现进一步解耦
