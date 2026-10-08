# 04 — RAII: File Descriptor Ownership

## 今日目标
理解 Resource Acquisition Is Initialization：资源获取与对象生命周期绑定。练习对象是 Linux 文件描述符的独占所有权。

## 任务
实现 `UniqueFd`，它拥有一个 fd，并在析构时调用 `close()`。这是 Linux/POSIX 练习，默认 C++17。

1. 构造：接收一个 fd，保存它；-1 表示无效 fd。
2. 析构：如果 fd 有效，调用 `::close(fd_)`，保证不重复关闭。
3. `get() const`：返回当前 fd。
4. `valid() const`：判断 fd 是否有效。
5. `reset(int new_fd = -1)`：关闭旧 fd 并接管新 fd；注意相同 fd 的情况。
6. `release()`：交出 fd 并将自身设为无效，不调用 close。
7. 禁止复制：使用 `= delete` 禁止拷贝构造和拷贝赋值。移动语义留待后续阶段。

## 验证
`main()` 已包含基本作用域与所有权测试。注意：`UniqueFd` 析构后，不能再使用原 fd。

## 思考题
- 为什么析构函数不应该随意抛异常？
- 为什么 fd 包装类不应该默认可复制？
- `release()` 和 `reset()` 的所有权语义有什么区别？
- `close()` 返回错误时，析构函数应该怎样处理？

不要使用智能指针替代本练习；下一阶段再学。
