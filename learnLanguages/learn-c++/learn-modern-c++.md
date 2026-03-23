# 现代 C++ 学习笔记

本笔记记录了对现代 C++ 代码库的分析与学习，旨在通过实际案例理解现代 C++ 特性的应用。

## Example 1: 算法管理系统分析

本章分析了在 `example1` 项目（算法管理系统）中观察到的现代 C++ 使用模式。该代码展示了如何用 C 兼容接口封装 C++ 逻辑，同时利用现代 C++ 特性来提高安全性和可维护性。

### 1. 智能指针与内存管理

项目使用 `std::shared_ptr` 来管理算法实例的生命周期，避免了手动的 `new` 和 `delete` 调用。

*   **`std::shared_ptr`**: 用于共享 `AlgBase` 实例的所有权。
*   **`std::make_shared`**: 用于在一次分配中高效地创建实例。

#### 常见类型与语义

*   **`std::unique_ptr`**: 独占所有权（exclusive ownership）。对象只能有一个拥有者，支持移动（move）但不支持拷贝（copy）。它开销低、语义清晰，是现代 C++ 中动态分配对象的默认首选。
*   **`std::shared_ptr`**: 共享所有权（shared ownership）。通过引用计数（reference counting）管理对象生命周期，适合多个对象或模块需要共同持有同一资源的场景。
*   **`std::weak_ptr`**: 弱引用（weak reference）。不拥有对象，不增加引用计数，通常用于观察由 `std::shared_ptr` 管理的对象，或打破循环引用（cyclic reference）。

#### 从 C++11 到 C++20 的演进

智能指针在 C++11 到 C++20 之间逐步补齐了使用方式：

*   **C++11**: 标准化了 `std::unique_ptr`、`std::shared_ptr`、`std::weak_ptr`，并提供 `std::make_shared`。
*   **C++14**: 增加 `std::make_unique`，使 `unique_ptr` 的创建方式更自然、更安全。
*   **C++17**: 智能指针本体新增不多，但围绕 `std::weak_ptr`、`std::enable_shared_from_this` 的工程实践更加成熟，特别常见于回调、异步任务和对象生命周期管理。
*   **C++20**: 补充了 `std::make_shared<T[]>`、`std::make_shared_for_overwrite`，并标准化了 `std::atomic<std::shared_ptr<T>>` 的用法，使其在数组、性能优化和并发场景中更实用。

#### 创建方式的现代风格

现代 C++ 通常不推荐直接写裸 `new`，而是优先使用工厂函数：

```cpp
// 独占所有权，C++14 起推荐
auto p1 = std::make_unique<Algorithm>(instance_id);

// 共享所有权，C++11 起推荐
auto p2 = std::make_shared<Algorithm>(instance_id);

**示例 (`alg_manager.cpp`):**

```cpp
// 高效创建智能指针
auto algorithm = std::make_shared<Algorithm>(instance_id);
instances_.emplace(instance_id, algorithm);
```

### 2. 基于 RAII 的并发控制

通过 `std::mutex` 和 `std::lock_guard` 实现线程安全。`std::lock_guard` 确保在退出作用域时自动解锁互斥量（RAII），从而防止发生异常时出现死锁。

**示例 (`alg_base.cpp`):**

```cpp
int AlgBase::Execute(const AlgoIn& in, AlgoOut& out, bool is_new_instance) {
    // 函数返回时自动释放锁
    std::lock_guard<std::mutex> lock(mutex_);

    // ... 临界区 ...
    return rc;
}
```

### 3. 类型推导与结构化绑定 (C++17)

代码利用 C++17 的结构化绑定，直接将 `std::pair` 或元组解包为命名变量，使代码更具可读性。

**示例 (`alg_exports.cpp`):**

```cpp
// 解包 pair<shared_ptr<AlgBase>, bool>
auto [instance, created] = manager().GetOrCreate(instance_id);
```

**示例 (`alg_manager.cpp`):**

```cpp
// 迭代器的自动类型推导
auto it = instances_.find(instance_id);
if (it != instances_.end()) {
    // ...
}
```

### 4. 现代类设计

类接口使用特定关键字来阐明意图并防止误用。

*   **`explicit`**: 防止单参数构造函数发生隐式转换。
*   **`override`**: 确保函数确实覆盖了基类中的虚函数。
*   **`final`**: 禁止类被继承或方法被重写。
*   **`= default`**: 显式要求使用编译器生成的默认实现。

**示例 (`algorithm.h`):**

```cpp
class Algorithm final : public AlgBase {
public:
    explicit Algorithm(std::string instance_id);
    ~Algorithm() override = default;

    // ...
    int DoInit(const AlgoIn& in, AlgoOut& out) override;
};
```

### 5. 编译期检查

使用 `static_assert` 在编译时强制执行假设，这对于在暴露 C 结构体时维护二进制兼容性 (ABI) 至关重要。

**示例 (`alg_exports.h`):**

```cpp
// 确保结构体大小符合预期，防止 ABI 破坏
ALGO_STATIC_ASSERT(sizeof(AlgoPoint) == 16u, "AlgoPoint ABI changed: expected size 16");
ALGO_STATIC_ASSERT(offsetof(AlgoPoint, ap) == 8u, "AlgoPoint ABI changed: expected ap offset 8");
```

### 6. 常量与内部链接

*   **`constexpr`**: 定义可在编译时求值的常量。
*   **匿名命名空间**: 用来代替 `static` 函数，将辅助函数和常量的作用域限制在当前翻译单元（文件）内，防止符号冲突。

**示例 (`algorithm.cpp`):**

```cpp
namespace {
    // 内部链接常量
    constexpr double kTuneTolerance = 1e-9;
}
```

### 7. C++ 与 C 的互操作性

项目使用 `extern "C"` 暴露 C API，允许支持 C 链接的其他语言或系统使用该库。

**示例 (`alg_exports.cpp`):**

```cpp
extern "C" int alg_run(const AlgoIn* in, AlgoOut* out) {
    // ... 实现 ...
}
```
