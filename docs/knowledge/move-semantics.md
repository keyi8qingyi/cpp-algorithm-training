# 引用与移动语义

本页以 C++17 为基准，关联 01、06 和 07 练习。右值引用、独占资源转移，以及手写移动与 vector 成员移动的对比已经在练习中使用；返回值、容器与异常保证的其他用法作为扩展参考。特殊成员函数的生成规则见 [Rule of Zero / Five](rule-of-zero-five.md)。

## 引用类型与表达式的值类别

引用是已有对象的别名。`T&` 是左值引用，`T&&` 是右值引用；这里讨论明确类型 T 的普通引用声明，模板中推导出的 `T&&` 还可能是转发引用，留待模板阶段。

| 写法 | 含义与限制 |
| --- | --- |
| `T& ref = object` | 绑定兼容的非 const 左值，可通过引用修改对象 |
| `const T& ref = expression` | 只读借用；可以绑定左值，也可以绑定临时对象 |
| `T&& ref = temporary` | 绑定兼容的右值，本身不执行资源转移 |
| `T&& ref = std::move(object)` | 绑定原对象，允许后续选择移动操作；不产生独立副本 |

“左值、右值”描述表达式，不是简单地按赋值号左边或右边划分。具名变量表达式通常是左值；右值包括纯右值和亡值。`42` 是纯右值；对普通对象调用 `std::move(object)` 得到亡值表达式，仍表示原对象。

```cpp
#include <cassert>
#include <utility>

int main() {
    int value = 10;
    int& alias = value;
    int&& movable = std::move(value);
    movable = 20;
    assert(alias == 20 && &movable == &value);
    const int& temporary = 42;
    assert(temporary == 42);
}
```

局部引用直接绑定某些临时对象时可以延长临时对象寿命，但不要推广成“引用都能延长对象寿命”。返回局部对象引用仍会悬空；函数调用中绑定到引用参数的临时对象通常只活到包含该调用的完整表达式结束，保存参数引用不能让它一直存活。

## std::move：允许移动，不亲自搬运

头文件 `<utility>`。`std::move(expression)` 返回去掉引用后的类型对应的右值引用，保留 const 限定。对非 const 的 T 左值，结果为 `T&&`；对 const T 左值，结果为 `const T&&`。它不分配内存、不复制数组、不释放资源，也不会自动清空对象。

随后被选择的构造函数、赋值运算符或其他函数，决定实际如何处理资源。一个类型没有可用的移动构造时，右值也可能绑定到 `const T&` 并发生复制；调用 `std::move` 并不保证最终一定发生移动。

```cpp
#include <cassert>
#include <string>
#include <utility>

int main() {
    const std::string original = "log";
    std::string copy(std::move(original)); // const 不会消失，这里复制
    assert(copy == "log" && original == "log");

    std::string source = "buffer";
    std::string destination(std::move(source));
    assert(destination == "buffer");
    source = "reused"; // 移动后的对象仍可重新赋值
    assert(source == "reused");
}
```

对于名为 `other` 的右值引用参数，表达式 `other` 自身是左值。要将其继续传给要求右值的接口，可能还需 `std::move(other)`。不要仅因看到变量声明里有 `&&`，就认定任何使用它的表达式都是右值。

## 构造、赋值与资源所有权

下表的 T 是一个提供相应特殊成员函数的类；实际是否支持某种操作由类定义决定。

| 用法 | 对应操作 | 来源与目标 |
| --- | --- | --- |
| `T b(a)` / `T b = a` | 拷贝构造 `T(const T&)` | 创建新对象；资源类若支持独立复制，需安排独立资源 |
| 已有 b 后执行 `b = a` | 拷贝赋值 `T& operator=(const T&)` | 目标已存在，必须处理已有状态 |
| `T b(std::move(a))` / `T b = std::move(a)` | 移动构造 `T(T&&)` | 新对象从来源取得可移动状态 |
| 已有 b 后执行 `b = std::move(a)` | 移动赋值 `T& operator=(T&&)` | 目标先处理原有资源，再接管来源资源 |

对于本题的独占数组，移动只转移地址和大小，数组仍在原位置。不同对象间移动后，来源被明确置为 `nullptr` 和 `0`；它仍存在，可以析构、查询空状态和重新赋值。其他类型须查各自约定：标准库类型通常保证有效但状态未指定，不能普遍断言一定为空。用户自定义类型的移动行为由实现负责。

移动赋值中的 `this == &other` 比较目标与来源是否是同一对象。本题在释放资源前检查，自移动直接返回，保持原数据。`return *this` 返回目标对象的引用，不会额外复制对象。

标准对象的移动行为不一定都等于“接管一个指针”：整数的移动效果与复制值相同；类可以逐个移动成员；某些容器还受到分配器条件影响。O(1) 是本题转移整数数组所有权的性质，不是所有移动操作的复杂度保证。

## noexcept 与禁用操作

- `T(T&&) noexcept` 承诺异常不会逃出函数；若实际逃出，则调用 `std::terminate`。它不负责自动捕获异常。
- `noexcept(expression)` 是查询表达式是否可能抛异常的运算符，得到 bool；与函数声明上的异常说明位置和用途不同。
- `T(const T&) = delete` 与删除拷贝赋值一起禁止复制，但不会自动实现移动。通过 `T&` / `const T&` 借用对象仍合法。
- 不抛异常的移动构造有利于容器扩容时保留异常保证。当复制可用而移动可能抛异常时，vector 可能选择复制；只允许移动且移动可能抛异常的类型要另外分析其保证。

不要在本可通过接管指针完成的 noexcept 移动函数里重新 `new[]` 数组并复制元素：这增加工作量，还引入分配异常。本次实际错误见 [学习笔记第 06 节](../learning-notes.md#06--值类别与移动语义)。

## 容器插入与返回值

已有 `unique_ptr` 插入 `vector` 时，可写 `items.push_back(std::move(ptr))`，转移给容器元素。已经是临时对象时，通常直接传 `std::make_unique<T>(...)`，不需要再包装 std::move。见 [vector](vector.md) 和 [智能指针](smart-pointers.md)。

返回具名局部变量时通常直接写 `return result;`：编译器可执行 NRVO，符合条件时也可选择移动。手动写 `return std::move(result);` 会阻止这一种具名返回值优化。

```cpp
#include <cassert>
#include <memory>

std::unique_ptr<int> make_value() {
    auto result = std::make_unique<int>(42);
    return result;
}

int main() {
    auto value = make_value();
    assert(value && *value == 42);
}
```

直接返回同类型纯右值（例如 `return T(args);`）在 C++17 有相应的保证复制消除规则；具名局部对象的 NRVO 仍不是强制保证。函数返回引用则是借用，必须另行保证被引用对象活得足够久，不能用 std::move 修复悬空引用。

## 原始数组接口与生命周期

06 练习用 `new int[n]{}` 申请并零初始化 n 个整数，返回 `int*`，失败可抛异常；用 `delete[]` 配对释放。n 为 0 合法，返回指针不保证为空，不能解引用，但可释放。`delete[] nullptr` 安全。

`data()` 返回的裸指针通常表示借用；是否有所有权要看接口约定，不能看到指针就自行 delete。数组成功移动后，已有的元素指针仍可能有效，但它指向的内存已由新对象管理；新所有者释放数组后，它会悬空。`size() == 0` 时没有有效下标，空状态可析构不等于仍可读取元素。
