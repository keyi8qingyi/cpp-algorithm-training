# 05 — Smart Pointers

## 本阶段目标
在已经掌握 RAII 和独占 fd 所有权的基础上，学习现代 C++ 的动态对象所有权。

- `std::unique_ptr`：独占所有权，禁止复制，可以移动
- `std::shared_ptr`：共享所有权与引用计数
- `std::weak_ptr`：非拥有型观察，避免循环引用
- `std::make_unique` / `std::make_shared`
- `const` 引用、裸指针与智能指针参数分别表达什么

## Exercise — Session / SessionManager

模拟 Linux 服务进程中的 Session 管理。不涉及真实网络连接。

### Task 1 — 独占所有权
完成 `make_session`，创建并返回一个 `std::unique_ptr<Session>`。调用方应拥有对象；禁止返回指向局部对象的裸指针。

### Task 2 — 转移所有权
完成 `SessionManager::add`，接收 `std::unique_ptr<Session>` 并放入容器。注意不能复制 `unique_ptr`。实现 `count() const` 和 `print_all() const`。

### Task 3 — 共享与观察
完成 `SharedSessionRegistry`：
- `publish` 接收 `std::shared_ptr<Session>` 并保存；
- `observe` 返回一个 `std::weak_ptr<Session>`；
- `clear` 移除 Registry 的共享所有权；
- 调用方通过 `lock()` 判断对象是否仍然存活。

### Task 4 — 观察生命周期
运行 main 中的测试，确认 Session 只在最后一个所有者释放时析构。

## 思考题
1. 为什么 `unique_ptr` 不允许复制，却可以转移？
2. `shared_ptr` 的引用计数和对象自身的引用有什么区别？
3. `weak_ptr` 为什么不延长对象生命周期？
4. 什么情况下函数参数应当是 `const Session&`，而不是 `shared_ptr<Session>`？
5. 什么时候应该避免使用 `shared_ptr`？

不要让 Codex 直接填写 TODO；先自己完成。默认 C++17。
