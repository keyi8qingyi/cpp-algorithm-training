# 08 — Templates: Generic System Components

## 今日目标
掌握 C++17 函数模板、类模板、模板实例化、const correctness，并理解模板在系统组件复用中的作用。

## Task 1 — 函数模板
完成 `clamp_value`：接收同类型的 value、low、high，将值限制在范围内。要求适用于 int 和 double；假设 low <= high。不要使用 std::clamp 直接代替本练习。

## Task 2 — 类模板：FixedBuffer<T, N>
模拟系统开发中的固定容量缓冲区。
- 使用 `std::array<T, N>` 存储元素，避免手动内存管理。
- `size() const` 返回容量。
- `at(index)` 提供 const 和非 const 重载，越界时抛出 `std::out_of_range`（可使用 array::at）。
- `fill(const T& value)` 填充缓冲区。
- 让 `FixedBuffer<int, 4>` 和 `FixedBuffer<char, 8>` 都可以使用。

## Task 3 — 类模板：Counter<T>
为 Linux 服务组件中的事件/统计计数提供可复用封装。
- 构造函数接受初始值。
- `increment(const T& delta)` 累加。
- `value() const` 返回当前值。
- 分别实例化 `Counter<int>` 与 `Counter<double>`。

## 思考
1. 为什么模板的实现通常放在头文件里？
2. `typename` 和 `class` 在类型模板参数位置有什么区别？
3. `FixedBuffer<T, N>` 中 T 与 N 分别属于哪种模板参数？
4. 类模板实例化与普通类定义有什么区别？
5. 什么时候模板会让接口更清晰，什么时候反而增加复杂度？

只实现 TODO，不要求手工维护根 README。默认 C++17。
