# 01 — References & const

## 本阶段目标

从熟悉的 C 指针思维开始，掌握 C++ 中最常见的引用与 const 用法。

完成后应能解释并实际使用：

- `T&` 与 `const T&`
- 值传递、指针传递、引用传递的区别
- 为什么大型对象通常使用 `const T&` 只读传参
- `const` 放在变量、指针和引用附近分别意味着什么
- 返回值与返回引用的基本区别
- 什么情况下不能返回局部变量的引用

## Exercise

打开 `exercise.cpp`，完成其中所有 TODO。

要求尽量使用 C++ 的引用和 const，而不是把所有接口都写成 C 风格指针。

### Task 1 — swap_values

使用引用实现两个整数的交换。

限制：函数参数不能使用指针。

### Task 2 — sum_buffer

计算一个 `std::vector<int>` 中所有元素之和。

要求：

- 不复制整个 vector；
- 函数不能修改传入的 vector；
- 使用 range-based for。

### Task 3 — BufferView

实现一个很小的 `BufferView` 类：

- 构造时接收一个外部 `std::vector<int>`；
- `size()` 返回元素数量；
- `at(index)` 提供只读访问；
- `set(index, value)` 可以修改原始 vector 中对应元素。

这里重点观察：引用成员、const 成员函数以及对象和底层数据之间的关系。

## 思考题

完成代码后，尝试自己回答：

1. `const std::vector<int>&` 为什么通常比 `std::vector<int>` 更适合作为只读函数参数？
2. `const int* p` 与 `int* const p` 的区别是什么？
3. 为什么返回局部变量的引用是危险的？
4. `BufferView` 为什么不拥有 vector 的生命周期？

不要求把答案写进代码；后续 review 时会结合你的实现讨论。
