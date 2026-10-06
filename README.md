# LeetCode Training

一个以 GitHub 作为唯一进度源的日常算法训练仓库。

## 工作流

1. 每个工作日 14:00，ChatGPT 检查仓库中的完成情况并选择下一道题。
2. 本地执行 `git pull` 获取当天题目。
3. 独立完成 `problems/<题号>-<slug>/solution.cpp`。
4. 完成后提交并推送到 `main`。
5. ChatGPT 下一次扫描时根据代码和提交记录判断完成情况，并据此调整后续题目。

## 完成标准

一道题满足以下条件时视为已完成：

- 对应目录中存在 `solution.cpp`；
- `solution.cpp` 已包含实际实现，而不是初始 TODO 模板；
- 实现已经 commit 并 push 到 GitHub。

不要求手动修改本 README 来登记进度，GitHub 中的题目目录和提交记录就是进度源。

## 目录结构

```text
problems/
└── 0001-two-sum/
    ├── README.md
    └── solution.cpp
```

每道题的 README 只包含题目编号、名称、难度、核心考点和 LeetCode 链接，不提前保存解法。

## 训练原则

训练主题会逐步覆盖：数组与哈希、双指针、滑动窗口、栈与队列、链表、二分查找、树、DFS/BFS、堆、回溯、动态规划和图。

优先建立常见算法模式，而不是随机刷题；后续选题会参考已完成题目、近期错误和薄弱主题。

## Commit 约定

推荐格式：

```text
solve: 1 two sum
```

如果是重新优化已有解法：

```text
refactor: 1 two sum
```
