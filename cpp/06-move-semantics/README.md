# 06 — Value Categories & Move Semantics

## 本阶段目标
掌握 C++17 的左值、右值、移动构造、移动赋值和 std::move，并理解移动操作如何转移资源所有权。

## 任务：MovableBuffer
这次实现一个独占动态数组的 Buffer。不要使用 std::vector 或智能指针代替数组资源，目的是观察所有权转移。使用 new[] / delete[]，默认 C++17。

1. 构造与析构：申请并释放整数数组，支持 size=0。
2. 禁止复制：删除拷贝构造和拷贝赋值，避免意外深拷贝。
3. 移动构造：实现 Buffer(Buffer&& other) noexcept，将资源转移到新对象，并使 other 处于可析构、可重新赋值的空状态。
4. 移动赋值：实现 Buffer& operator=(Buffer&& other) noexcept，处理原有资源、自移动赋值，并转移所有权。
5. size() const、data() 的 const / 非 const 重载、set() 和 get()：完成访问接口。测试仅访问有效下标，越界检查不是本阶段重点。
6. 运行 main，观察 move 后原对象的状态以及析构行为。

## 思考题
- std::move 到底执行了什么？为什么它本身不搬运内存？
- Buffer&& other 虽然是右值引用，表达式 other 在函数体里属于左值还是右值？
- 为什么移动构造和移动赋值通常标注 noexcept？
- 为什么 move 后的对象必须仍然可安全析构？
- Buffer a = std::move(b) 和 a = std::move(b) 分别调用什么？

完成后 push，等待 review。Codex 可解释语法和编译错误，但不要直接生成 TODO 答案。
