# std::vector：接口与用法

头文件：`<vector>`。`std::vector<T>` 拥有元素，适合需要按下标随机访问、主要在末尾增删的可变长序列。除 `vector<bool>` 特化外，元素连续存储；不要把 `vector<bool>` 当作普通的 `bool` 数组使用。

01 练习使用了引用、`size()`、`at()`，05 使用了 `vector<unique_ptr<Session>>`。以下还包含容量、增删与迭代器的扩展参考，不表示这些内容都已完成训练。

## 创建与赋值

```cpp
#include <vector>

int main() {
    std::vector<int> empty;          // 0 个元素
    std::vector<int> zeros(3);       // 3 个 0
    std::vector<int> repeated(3, 7);  // 3 个 7
    std::vector<int> values{3, 7};    // 2 个元素：3、7
    std::vector<int> copy = values;  // 独立复制元素
    copy.assign(4, 9);               // 替换为 4 个 9
}
```

`vector(first, last)` 从迭代器区间 `[first, last)` 创建容器；`assign(first, last)` 用区间内容替换已有元素。这些操作要求元素满足相应的构造、赋值条件；例如不能复制 `vector<unique_ptr<T>>` 的元素。

## 查询、访问与容量

| 接口 | 参数与返回值 | 使用边界 |
| --- | --- | --- |
| `size()` / `empty()` | 无参数；返回 `size_type` / `bool` | 分别表示现有元素数 / 是否无元素；O(1) |
| `capacity()` | 无参数；返回无需重新分配即可容纳的元素数 | 容量至少为大小，额外容量不是可访问的元素 |
| `v[i]` | 下标；返回 `T&`，const 容器返回 `const T&` | C++17 不检查越界，必须 `i < size()`；O(1) |
| `at(i)` | 下标；返回元素引用，const 重载只读 | 越界抛 `std::out_of_range`；O(1) |
| `front()` / `back()` | 无参数；返回首尾元素引用 | 要求非空，const 容器得到只读引用 |
| `data()` | 无参数；返回首元素位置的 `T*` / `const T*` | 借用地址，不得 `delete[]`；空容器的结果不可解引用，也不保证为空 |
| `reserve(n)` | 期望容量；返回 `void` | 需要时扩容，**不改变 size**，也不创建额外元素；不会缩容 |
| `resize(n)` / `resize(n, value)` | 新大小、可选填充值；返回 `void` | 缩小时销毁末尾元素；增大时构造新元素，`vector<int>` 的单参数版本补 0 |
| `shrink_to_fit()` | 无参数；返回 `void` | 非强制的缩容请求，不能保证 capacity 最终等于 size |

访问现有元素时用 `size()` 判断边界。`reserve(100)` 之后若大小仍为 0，写 `v[0]` 依然越界；应使用 `push_back` 加入元素，或先 `resize`。

分配可能抛 `std::bad_alloc`；超过允许大小的请求可能抛 `std::length_error`；元素自身的构造或复制也可能抛异常。`reserve` 只在需要重新分配时搬迁现有元素，不要在每次追加前都调用 `reserve(size() + 1)`，这可能使反复增长变得低效。

## 增删与遍历

| 接口 | 参数与返回值 | 代价或约束 |
| --- | --- | --- |
| `push_back(value)` | 接收 `const T&` 或 `T&&`；返回 `void` | 在末尾复制或移动一个元素，均摊 O(1) |
| `emplace_back(args...)` | 接收元素构造参数；C++17 返回新元素的 `T&` | 在末尾构造元素；扩容仍可能搬迁旧元素，不保证比 push_back 更快 |
| `insert(pos, value)` | 在有效位置迭代器前插入；此重载返回新元素迭代器 | `pos` 可为 `end()`；通常 O(n)，不是整数下标参数 |
| `erase(pos)` / `erase(first, last)` | 删除元素 / 半开区间；返回删除区域之后的迭代器 | 删除单个元素时不能传 `end()`；通常 O(n) |
| `pop_back()` | 无参数；返回 `void` | 要求非空，销毁末尾元素；不会返回被删除的值 |
| `clear()` | 无参数；返回 `void` | 销毁全部元素，size 变为 0，保留容量；O(n) |
| `begin()` / `end()` | 无参数；返回首位置 / 尾后位置迭代器 | 尾后位置不能解引用；空容器二者相等 |
| `cbegin()` / `cend()` | 无参数；返回只读迭代器 | 即使容器本身非 const，也不能经这些迭代器修改元素 |
| `rbegin()` / `rend()` | 无参数；返回反向遍历边界 | 使用 `++it` 向原序列前方遍历；`rend()` 不可解引用 |

`for (auto value : v)` 复制每个元素；`for (auto& value : v)` 可修改元素；`for (const auto& value : v)` 避免复制并限制经该引用修改元素。对于智能指针，指针对象的 const 与所指对象的 const 不同，见 [智能指针](smart-pointers.md)。

按条件删除时，使用 `erase` 返回的有效后继：

```cpp
#include <vector>

int main() {
    std::vector<int> values{2, -1, 4, -3};
    for (auto it = values.begin(); it != values.end();) {
        if (*it < 0) {
            it = values.erase(it);
        } else {
            ++it;
        }
    }
}
```

这个示例便于理解迭代器，但反复删除可能是 O(n²)。批量过滤还可学习 `<algorithm>` 的 `remove_if` 配合 `erase`；`std::erase_if(vector, ...)` 是 C++20 接口，不属于本仓库默认 C++17。

## 指针、引用和迭代器何时失效

**重新分配存储会使指向原元素的全部指针、引用和迭代器失效。** `reserve` 的重分配规则和 `erase` 的失效规则可查 [vector 官方接口参考](https://learn.microsoft.com/en-us/cpp/standard-library/vector-class?view=msvc-170)。

| 操作 | 没有重新分配时 | 发生重新分配时 |
| --- | --- | --- |
| `push_back` / `emplace_back` | 原元素的引用和迭代器仍有效，旧 `end()` 失效 | 全部失效 |
| `insert` | 插入点及之后的引用和迭代器失效，包括旧 `end()` | 全部失效 |
| `erase` | 删除点及之后的引用和迭代器失效，包括旧 `end()` | 不适用 |
| `reserve` | 都保持有效 | 全部失效 |
| `resize` | 缩小时被移除元素及其后的位置失效；增大时旧 `end()` 失效 | 全部失效 |
| `clear` | 所有元素引用、指针和迭代器以及旧 `end()` 失效 | 不适用 |
| `shrink_to_fit` | 保持有效 | 全部失效 |

不要保存 `auto& item = v[0]` 后追加元素，再假定 `item` 仍有效。也不要在 range-for 中增删同一个 vector，却继续使用循环内部保存的迭代器。

## 存放不可复制的对象

```cpp
#include <memory>
#include <utility>
#include <vector>

int main() {
    std::vector<std::unique_ptr<int>> items;
    auto owned = std::make_unique<int>(42);
    items.push_back(std::move(owned)); // 元素接管所有权，owned 变空
    items.push_back(std::make_unique<int>(7));
    items.clear();                   // 销毁 unique_ptr，同时释放两个 int
}
```

扩容后 `unique_ptr` 元素本身的位置可能改变，但它管理的动态 `int` 不会因此搬家。`vector<int*>` 的行为不同：销毁容器只销毁指针值，不会替你释放指向的整数。
