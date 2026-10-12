# 07 — Rule of Zero / Five

## 本阶段目标
把前面学过的析构、拷贝、移动、资源所有权串成一套类设计原则。

- Rule of Five：需要手动管理独占资源时，考虑析构、拷贝构造、拷贝赋值、移动构造、移动赋值五种特殊成员函数
- Rule of Zero：把资源交给标准库 RAII 成员，业务类尽量不手写这些函数
- 深拷贝与独占资源的不同语义
- noexcept、const correctness、异常安全

## Exercise

### Task 1 — LegacyBuffer：Rule of Five
实现一个拥有动态 int 数组的 LegacyBuffer。要求：
- 构造与析构正确管理 new[] / delete[]
- 拷贝构造、拷贝赋值实现深拷贝
- 移动构造、移动赋值转移所有权并清空源对象
- 自赋值和自移动安全
- 访问接口 size() const、at() const / 非 const

### Task 2 — ModernBuffer：Rule of Zero
使用 std::vector<int> 作为唯一数据成员实现 ModernBuffer。
- 不手写析构、拷贝、移动特殊成员函数
- 完成构造、size、at 接口
- 验证复制后两份数据互不影响，并验证移动后的目标数据正确

### Task 3 — 对比
观察两种类的代码量、所有权和异常安全差异。请不要把 LegacyBuffer 直接改为 vector；两种方案都需要保留。

## 思考题
1. 为什么写了析构函数以后，隐式移动行为会受到影响？
2. Rule of Five 是要求所有类都手写五个函数吗？
3. 为什么 ModernBuffer 不需要手写析构函数？
4. std::vector 的复制和移动分别是什么语义？
5. 如果类拥有一个独占的 Linux fd，它一定应该支持复制吗？

默认 C++17。先自行实现，Codex 仅作语法解释或 Review。
