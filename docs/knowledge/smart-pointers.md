# 智能指针：所有权与接口

头文件：`<memory>`；使用 `std::move` 另外包含 `<utility>`。05 已练习基本创建、容器存储、共享和观察；数组、自定义删除器及更完整的参数设计作为扩展参考。

## 如何选择

| 类型 | 表达的关系 | 复制 / 移动 | 对象什么时候销毁 |
| --- | --- | --- | --- |
| `unique_ptr<T>` | 独占所有权 | 不可复制，可以移动 | 持有者析构或 reset 时调用删除器 |
| `shared_ptr<T>` | 共享所有权 | 复制共享所有权；移动转交已有份额 | 最后一个共享所有者释放时 |
| `weak_ptr<T>` | 观察共享对象 | 可以复制或移动观察关系 | 不决定对象寿命；访问前 lock |
| `T&` / `T*` | 通常表达借用 | 复制别名或地址，不增加所有者 | 由实际所有者决定；裸指针的约定需由接口说明 |

优先明确谁负责释放对象，再决定指针类型。多个地方需要访问，不自动意味着需要共享所有权。

## unique_ptr：创建、借用、转交与释放

下表以普通对象和默认删除器为主。带自定义删除器时，`pointer` 类型不一定就是 `T*`。

| 接口 | 参数与返回值 | 所有权含义 |
| --- | --- | --- |
| `make_unique<T>(args...)` | 把 args 用于构造 T，返回 `unique_ptr<T>` | 创建并拥有对象，构造或分配可能抛异常 |
| `make_unique<T[]>(n)` | 数组元素数，返回 `unique_ptr<T[]>` | 值初始化 n 个元素；整数初始化为 0，无 size 方法 |
| `p.get()` | 无参数，返回保存的指针 | 只借用，不交出释放责任 |
| `if (p)` | 检查保存的指针是否非空 | 解引用前判断；不对外部错误删除提供保护 |
| `*p` / `p->member` | 访问对象，前者返回对象引用 | 要求非空；数组版本用 `p[i]`，不检查越界 |
| `q = std::move(p)` | 接收另一个独占指针，返回 q 的引用 | q 原对象被释放，再接管 p；不同对象间转移后 p 为空 |
| `p.release()` | 无参数，返回原指针 | p 变空但对象仍存活，调用方接手释放责任 |
| `p.reset()` / `p.reset(raw)` | 可选新指针，返回 void | 释放旧对象，变空或接管 raw |
| `p.swap(q)` | 接收另一个兼容 unique_ptr，返回 void | 交换指针和删除器，不复制所管理对象 |

`get`、`release` 和 `reset` 的区别是借用、交出所有权、替换或释放资源；声明可查 [unique_ptr 官方参考](https://learn.microsoft.com/en-us/cpp/standard-library/unique-ptr-class?view=msvc-170)。

```cpp
#include <cassert>
#include <memory>
#include <utility>

int main() {
    auto p = std::make_unique<int>(42);
    int* borrowed = p.get();
    auto q = std::move(p);
    assert(!p && q.get() == borrowed);

    int* raw = q.release();
    assert(!q);
    std::unique_ptr<int> owner(raw); // 立即交给新的所有者
    owner.reset();                  // 此后 borrowed 和 raw 都悬空

    auto values = std::make_unique<int[]>(3);
    assert(values[0] == 0);
}
```

常见错误是 `delete p.get()`、从同一个裸指针建立两个独占所有者，或者 `p.reset(p.get())`。最后一种会释放自己仍保存的地址，不具备练习 `UniqueFd::reset` 的自相等保护。`release()` 通常用于必须接管裸指针的接口互操作，普通所有权转移直接使用移动更清晰。

## 自定义删除器

资源的释放函数不一定是 delete。例如 C 的 `FILE*` 必须用 `fclose`：

```cpp
#include <cstdio>
#include <memory>

struct FileCloser {
    void operator()(std::FILE* file) const noexcept {
        if (file) std::fclose(file);
    }
};

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    std::unique_ptr<std::FILE, FileCloser> file(std::fopen(argv[1], "r"));
    if (!file) return 1;
    // file 离开作用域时调用 fclose；这里的删除器忽略关闭错误
}
```

删除器必须与资源来源匹配，清理时不应让异常逸出。如果需要获知写文件最终的关闭错误，应设计显式关闭操作。`make_unique` 使用默认删除器，不能靠给它多传一个参数指定删除器。

## shared_ptr：共享控制块

| 接口 | 参数与返回值 | 所有权含义 |
| --- | --- | --- |
| `make_shared<T>(args...)` | 构造参数，返回 `shared_ptr<T>` | 创建对象及其共享控制块；分配或构造可能抛异常 |
| `auto q = p` | 复制 shared_ptr | 增加共享所有者份额，不复制 T 对象 |
| `auto q = std::move(p)` | 移动 shared_ptr | 转交所有权份额，p 变空 |
| `get()` / `*p` / `p->member` | 借用地址 / 对象引用 / 成员访问 | 与 unique_ptr 一样不交出所有权，不得手动 delete |
| `reset()` | 无参数，返回 void | 放弃当前份额，仅在最后一个所有者离开时销毁对象 |
| `use_count()` | 无参数，返回 long | 观察共享所有者数量；不能作为并发访问对象的安全判断 |
| `swap(q)` | 另一 shared_ptr，返回 void | 交换保存的指针及所有权关系 |

`shared_ptr` 没有 `release()`：多个所有者可能同时存在，不能像 unique_ptr 那样单方面交出完整释放责任。不要用 `shared_ptr<T>(p.get())` 从已受管理的对象重新建立独立控制块，否则会重复释放。复制一个已有 shared_ptr 才是在共享同一控制块。接口声明见 [shared_ptr 官方参考](https://learn.microsoft.com/en-us/cpp/standard-library/shared-ptr-class?view=msvc-170)。

不同 shared_ptr 对象共享控制块时，计数操作有并发保障；这不代表所指对象 T 自动线程安全，也不代表可以不加同步地读写同一个 shared_ptr 变量。C++17 没有 C++20 的 `std::atomic<std::shared_ptr<T>>` 特化。

本页数组创建示例使用 `make_unique<T[]>`；`make_shared<T[]>(n)` 的数组工厂重载是 C++20 才提供的，不应直接用于本仓库的 C++17 示例。

## weak_ptr：访问时再取得所有权

| 接口 | 参数与返回值 | 用法 |
| --- | --- | --- |
| `weak_ptr<T> w(p)` | 从兼容的 shared_ptr 建立观察 | 不增加共享所有者数量 |
| `w.lock()` | 无参数；返回 shared_ptr | 成功则在返回指针持有期间延长寿命，已销毁则返回空指针；不抛异常 |
| `w.expired()` | 无参数；返回 bool | 表示是否已无共享所有者；先检查它再访问不能代替 lock |
| `w.use_count()` | 无参数；返回 long | 查询观察目标的共享所有者数量，仅用于观察 |
| `w.reset()` | 无参数；返回 void | 放弃观察关系，不直接销毁目标对象 |

`weak_ptr` 没有可用于访问对象的 `operator->` 或 `operator*`；用 `lock` 保持临时所有权。接口见 [weak_ptr 官方参考](https://learn.microsoft.com/en-us/cpp/standard-library/weak-ptr-class?view=msvc-170)。

```cpp
#include <cassert>
#include <memory>

int main() {
    auto owner = std::make_shared<int>(7);
    std::weak_ptr<int> observer = owner;
    {
        auto held = observer.lock();
        assert(held && *held == 7);
        owner.reset();
        assert(!observer.expired()); // held 仍持有对象
    }
    assert(observer.expired());
    assert(!observer.lock());
}
```

如果两个对象互相用 shared_ptr 持有对方，它们可能形成循环，外部指针销毁后对象仍不析构。把其中表示观察或反向关系的一侧设计为 weak_ptr，才不会让它增加强所有者数量。对象销毁后，控制块还可能因观察者存在而保留；控制块寿命与对象寿命不同。

## const 与函数参数设计

| 写法 | 表达的意图 |
| --- | --- |
| `void inspect(const T& value)` | 调用期间只读借用对象，无需共享所有权 |
| `void modify(T& value)` | 借用并修改对象 |
| `void inspect(const T* value)` | 可为空的只读借用，函数需定义空值的含义 |
| `void consume(std::unique_ptr<T> value)` | 接收独占所有权，已有指针调用时需移动 |
| `void replace(std::unique_ptr<T>& value)` | 允许修改调用方的指针，例如 reset 或重新赋值 |
| `void retain(std::shared_ptr<T> value)` | 接收可保留的共享所有权；从左值传入会增加所有者份额 |
| `void observe(std::weak_ptr<T> value)` | 保存观察关系，访问时 lock |

`const std::unique_ptr<T>&` 限制的是指针对象，通常仍能通过它修改 T；要表达所指对象只读，用 `unique_ptr<const T>` 或更直接的 `const T&` 借用。`shared_ptr` 同理。不要仅为了访问 T 的一个成员就强迫调用方使用某一种智能指针。

与练习的联系：`SessionManager::add` 转移 unique_ptr；`SharedSessionRegistry::publish` 保存共享所有权；`observe` 提供 weak_ptr。实际错误与思考题见 [学习笔记第 05 节](../learning-notes.md#05--智能指针与动态对象所有权)。
