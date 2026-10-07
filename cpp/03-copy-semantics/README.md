# 03 — Copy Constructor & Copy Assignment

## 本阶段目标

理解 C++ 对象发生“复制”时到底复制了什么，以及为什么拥有资源的类不能总依赖编译器生成的默认复制行为。

完成后应能解释并使用：

- 拷贝构造函数 `T(const T& other)`
- 拷贝赋值运算符 `T& operator=(const T& other)`
- “创建一个新对象”和“给已经存在的对象赋值”的区别
- 深拷贝与浅拷贝
- 自赋值 `a = a`
- 为什么资源所有权会让复制语义变复杂

## Exercise：Buffer

实现一个简化的 Buffer。为了让“拷贝”问题足够明显，本练习暂时故意使用 `new[] / delete[]` 管理一块动态内存。

> 这不是推荐的现代 C++ 写法。后面的 RAII、智能指针和 Rule of Zero 会逐步把它改造成更安全的实现。今天使用裸指针，是为了真正看见复制资源时发生了什么。

### Task 1 — 普通构造与析构

完成构造函数和析构函数：

- 构造一个指定大小的缓冲区；
- 使用 `new int[size]` 分配资源；
- 将元素初始化为 0；
- 析构时正确释放数组。

### Task 2 — 拷贝构造函数

实现：

```cpp
Buffer(const Buffer& other);
```

要求新 Buffer：

- 拥有自己独立的一块内存；
- 大小与 `other` 相同；
- 元素内容与 `other` 相同。

修改新对象不能影响原对象。

### Task 3 — 拷贝赋值运算符

实现：

```cpp
Buffer& operator=(const Buffer& other);
```

注意它面对的是一个**已经构造完成、可能已经拥有资源**的对象。

要求：

- 正确处理原有资源；
- 复制 `other` 的数据；
- 能处理 `buffer = buffer`；
- 返回 `*this`。

### Task 4 — 验证深拷贝

运行 main 中的测试，观察：

- `Buffer copied(original)` 调用的是谁；
- `assigned = original` 调用的是谁；
- 修改 copied 后 original 是否保持不变。

## 思考题

1. `Buffer b = a;` 和已经存在的 `b; b = a;` 为什么不是同一个操作？
2. 如果直接复制 `data_` 指针，会发生什么？
3. 两个对象共同保存同一个 `data_`，析构时可能发生什么？
4. 拷贝赋值为什么必须考虑目标对象原来已经拥有的内存？
5. 为什么 `operator=` 通常返回 `T&`？
6. `if (this == &other)` 在防什么？

完成后不要急着优化成 `std::vector` 或智能指针。今天的目标就是亲手处理一次资源复制。
