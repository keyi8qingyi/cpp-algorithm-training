# C++ & Algorithm Training

这个仓库用于渐进强化现代 C++ 编码能力，并以算法题作为配套训练。当前主线是 **C++ 特有语言能力**，算法训练是辅助，而不是单纯追求 LeetCode 数量。

## 工作流

1. 每个工作日 14:00，ChatGPT 扫描仓库中的 C++ 与算法训练进度。
2. 优先检查上一项 C++ 编码任务是否完成，再检查配套算法题。
3. 已完成时进行简短 code review，并布置下一阶段任务；未完成时不堆积新的主任务。
4. 本地 `git pull`，使用 Codex 辅助理解语法、编译错误和代码设计。
5. 自己完成代码后 commit + push；GitHub 是唯一进度源，不需要手工登记打卡。

## 学习路线

C++ 主线按以下顺序渐进：

1. 引用、const 与参数传递
2. 构造函数、析构函数、初始化列表与 const 成员函数
3. 拷贝构造、拷贝赋值与 Rule of Three
4. RAII 与资源所有权
5. `std::unique_ptr` / `std::shared_ptr` / `std::weak_ptr`
6. 左值、右值、移动构造、移动赋值与 `std::move`
7. Rule of Zero / Five
8. 函数模板与类模板
9. STL 容器、迭代器与 algorithms
10. Lambda、捕获与回调
11. Modern C++ 常用能力：`auto`、range-for、structured binding、`enum class`、`constexpr`、`optional` 等
12. 工程化：头文件/实现文件、namespace、异常安全、CMake、C/C++ 混合调用

练习对象尽量采用系统编程场景，例如 `Buffer`、`File`、`Socket`、`Timer`、`Connection`，避免只写教学型 `Student` / `Animal` 示例。

## 目录结构

```text
cpp/
└── 01-references-const/
    ├── README.md
    └── exercise.cpp

problems/
└── 0001-two-sum/
    ├── README.md
    └── solution.cpp
```

`cpp/` 是当前训练主线；`problems/` 用于配套算法练习。

## 完成标准

### C++ 训练

- `exercise.cpp` 中不存在尚未处理的核心 TODO；
- 程序体现当天要求练习的 C++ 特性；
- 代码已经 commit 并 push 到 GitHub。

### 算法题

- `solution.cpp` 已有实际实现；
- 不是初始 TODO 模板；
- 已 commit 并 push。

## Codex 的角色

Codex 可以直接帮助：C++ 语法、STL API、编译错误、语言机制解释、代码 review。

默认不要直接告诉用户算法题该使用什么算法，也不要主动补全核心算法；用户明确要求提示或答案时再放宽限制。

## Commit 约定

C++ 训练：

```text
cpp: finish references and const exercise
```

算法题：

```text
solve: 1 two sum
```
