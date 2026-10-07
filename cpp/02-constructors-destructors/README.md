# 02 — Constructors, Destructors & const Members

## 本阶段目标

今天重点不是“会写 class”，而是理解一个 C++ 对象从创建到销毁时发生了什么。

完成后应能实际使用并解释：

- 构造函数与析构函数什么时候执行
- 成员初始化列表与在构造函数体内赋值的区别
- 为什么某些成员必须在初始化列表中初始化
- `explicit` 的作用
- `const` 成员函数的含义
- 一个对象如何维护自己的合法状态
- 对象离开作用域时析构函数为什么会自动执行

## Exercise：Connection

实现一个简化的 Connection 类。它不进行真实网络通信，只模拟一个系统程序中“连接对象”的生命周期。

对象包含：

- `id_`：连接 ID，创建后不可改变；
- `name_`：连接名称；
- `connected_`：当前是否已连接。

### Task 1 — 构造函数

完成构造函数：

- 接收 `int id` 和 `std::string name`；
- 使用成员初始化列表初始化所有成员；
- `connected_` 初始为 `false`；
- 思考为什么 `id_` 如果声明为 `const int`，就必须使用初始化列表。

### Task 2 — const 成员函数

完成：

- `id() const`
- `name() const`
- `is_connected() const`

这些接口只能读取对象状态，不能修改对象。

对于 `name()`，思考返回 `std::string` 和 `const std::string&` 的差别。

### Task 3 — 修改对象状态

实现：

- `connect()`
- `disconnect()`

重复 connect/disconnect 不需要报错，只需要保证状态正确。

### Task 4 — 析构函数

在析构函数中打印：

```text
destroy connection: <id>
```

观察局部对象离开作用域时，析构函数在什么时候执行。

> 这里只用析构函数观察生命周期。真正的 C++ 中通常不会为了调试而让析构函数随意打印日志；后面的 RAII 模块会让析构函数真正负责资源释放。

## 思考题

完成后自己思考：

1. 构造函数体执行之前，成员变量是否已经存在？
2. 初始化列表的“初始化”和构造函数体中的“赋值”有什么区别？
3. 为什么 `const int id_` 不能先默认构造，再在构造函数体里赋值？
4. `bool is_connected() const` 最后的 `const` 修饰的是谁？
5. 如果 `name()` 返回 `const std::string&`，调用者拿到的是什么？它的生命周期受谁控制？

不要求把答案写进文件。完成代码并 push 后，ChatGPT 会结合实现 review。
