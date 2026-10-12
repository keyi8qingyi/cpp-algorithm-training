# 文件系统与文件 I/O

本页以 C++17 和 Linux 开发场景为基准。04 练习已经使用 POSIX fd 和 RAII；`std::filesystem`、文件流及读写循环是扩展参考，尚不代表完成了相关训练。

## 三类接口分别处理什么

| 类别 | 头文件 | 主要用途 | 生命周期 |
| --- | --- | --- | --- |
| `std::filesystem` | `<filesystem>` | 路径、文件属性、目录遍历、复制、重命名、删除 | `path` 只是路径值，不持有打开的文件 |
| C++ 文件流 | `<fstream>`，按需 `<string>` | 用 `ifstream` / `ofstream` 读取或写入文件内容 | 流对象以 RAII 管理打开的文件 |
| POSIX fd | `<fcntl.h>`、`<unistd.h>` | Linux 等 POSIX 平台的打开、读写、关闭 | fd 是整数句柄，需安排 `close`；本仓库用 `UniqueFd` 包装 |

`std::filesystem` 自 C++17 提供标准路径与文件系统操作；它不是内容读写流。见 [filesystem 官方概览](https://learn.microsoft.com/en-us/cpp/standard-library/filesystem?view=msvc-170)。

## path：路径拼接与拆分

示例中使用 `namespace fs = std::filesystem;`。

| 接口 | 参数与返回值 | 用法与边界 |
| --- | --- | --- |
| `fs::path(text)` | 从路径字符串创建 path | 不检查是否存在，不创建文件 |
| `base / child` | 返回拼接后的新 path | 拼接相对路径分量；若右侧是绝对路径，不能假定仍保留 base |
| `p /= child` | 修改 p，返回 `path&` | 拼接路径分量；与仅连接字符的 `p += text` 不同 |
| `parent_path()` / `filename()` | 无参数；返回 path | 分别取父路径部分 / 最后一个路径分量 |
| `stem()` / `extension()` | 无参数；返回 path | 例如 `report.txt` 得到 `report` / `.txt`；特殊名称需按规则理解 |
| `string()` | 无参数；返回 `std::string` | 用于显示或部分接口互操作；编码与平台有关，不是跨平台 UTF-8 保证 |
| `is_absolute()` | 无参数；返回 bool | 判断路径形式，不判断存在性 |
| `lexically_normal()` | 无参数；返回规范化后的 path | 只处理路径语法，不访问磁盘；不能据此解析符号链接或验证访问范围 |
| `fs::absolute(p)` | 返回绝对路径 | 相对路径依赖当前工作目录，不等于解析符号链接 |
| `fs::canonical(p)` | 返回解析符号链接及冗余分量后的规范路径 | 要求路径存在；错误须处理 |

```cpp
#include <cassert>
#include <filesystem>

int main() {
    namespace fs = std::filesystem;
    fs::path file = fs::path("logs") / "service.txt";
    assert(file.filename() == "service.txt");
    assert(file.stem() == "service");
    assert(file.extension() == ".txt");
    assert(file.parent_path() == "logs");
}
```

路径值拥有自己的字符数据；临时借用 `p.c_str()` 得到的指针不能在 p 销毁或相关修改后继续使用。路径分解与拼接的完整声明见 [path 官方参考](https://learn.microsoft.com/en-us/cpp/standard-library/path-class?view=msvc-170)。

## 查询与修改文件系统

下表的 p、from、to 均为路径；许多函数同时提供抛异常形式和最后接收 `std::error_code& ec` 的形式。

| 接口 | 返回值 | 应检查的条件 |
| --- | --- | --- |
| `exists(p)` | bool，路径是否存在 | false 不等于“任何查询错误”；带 ec 时同时检查 ec |
| `is_regular_file(p)` / `is_directory(p)` | bool，是否为普通文件 / 目录 | 通常跟随符号链接；查看链接本身可用 `symlink_status` |
| `file_size(p)` | `std::uintmax_t`，普通文件字节数 | 不用来递归统计目录；带 ec 的失败返回值是 `uintmax_t(-1)`，应优先查 ec |
| `create_directory(p)` | bool，是否新建目录 | 父目录需已存在；已是目录可返回 false，false 不必然是错误 |
| `create_directories(p)` | bool，是否新建目录 | 可创建缺失的父目录 |
| `copy_file(from, to, options)` | bool，是否实际复制 | 用于文件；可指定 `skip_existing` 或 `overwrite_existing` 等策略，不能组合相互冲突的同组策略 |
| `copy(from, to, options)` | void | 目录树复制需理解 `recursive`、链接和覆盖策略，默认并非复制所有后代 |
| `rename(from, to)` | void | 移动或重命名路径；跨文件系统等场景可能失败，不自动退化为复制 |
| `remove(p)` | bool，是否删除了一个文件或空目录 | 不存在可返回 false；非空目录不能用它递归删除 |
| `remove_all(p)` | `std::uintmax_t`，删除的条目数 | 递归删除；带 ec 的失败返回值为 `uintmax_t(-1)`，应查 ec |
| `current_path()` | 返回当前工作目录 path | `current_path(p)` 修改进程工作目录并返回 void，会影响后续相对路径解析 |

这些操作的返回值含义与重载声明可查 [filesystem 函数参考](https://learn.microsoft.com/en-us/cpp/standard-library/filesystem-functions?view=msvc-170)。查询和后续操作之间，文件系统可能已经变化；即使 `exists(p)` 刚返回 true，打开文件时仍须处理失败。

## 两种错误处理方式

不传 ec 的文件系统操作通常通过 `fs::filesystem_error` 报告操作系统错误；错误对象可查询 `code()`、`path1()` 和 `path2()`。传 ec 时通过错误码报告此类失败。**带 ec 不意味着所有调用链都绝不抛异常**，内存分配等行为仍须看具体函数的异常约定。

```cpp
#include <filesystem>
#include <iostream>
#include <system_error>

int main() {
    namespace fs = std::filesystem;
    std::error_code ec;
    const bool present = fs::exists("optional-config.txt", ec);
    if (ec) {
        std::cerr << "查询失败: " << ec.message() << '\n';
        return 1;
    }
    std::cout << (present ? "存在\n" : "不存在\n");
}
```

检查每次调用后的 ec，避免只检查 bool 或依赖之前的查询结果。使用覆盖、删除操作前，要理解传入路径及操作范围。

## 目录遍历

`directory_iterator(p)` 遍历直接子条目；`recursive_directory_iterator(p)` 递归遍历。解引用得到 `directory_entry`，可调用 `path()` 获取路径，也可查询 `is_regular_file()` 等属性。遍历顺序未指定，需要稳定顺序时自行收集并排序。

使用 ec 时，构造迭代器和推进迭代器都可能失败；不能只检查创建时的 ec，再依赖 range-for 中可能抛异常的 `++`。

```cpp
#include <filesystem>
#include <iostream>
#include <system_error>

int main() {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::directory_iterator it(".", ec), end;
    while (!ec && it != end) {
        std::cout << it->path() << '\n';
        it.increment(ec);
    }
    if (ec) {
        std::cerr << ec.message() << '\n';
        return 1;
    }
}
```

不要假定遍历会反映一个稳定快照。递归遍历默认不跟随目录符号链接；启用跟随选项后需考虑循环链接。要长期保存某条目的路径，复制其 `path`，不要保存迭代器推进后可能失效的条目引用。

## 文件流：读取内容

| 接口 | 参数、返回值 | 常见误用 |
| --- | --- | --- |
| `std::ifstream in(path)` | 按路径打开输入文件 | 默认失败时设置流状态，不会自动抛异常；打开后检查 `if (!in)` |
| `std::ofstream out(path)` | 打开输出文件 | 默认模式通常会截断已有文件；追加需指定 `std::ios::app` |
| `std::getline(in, line)` | 读取一行到 string，返回输入流引用 | 去掉分隔换行符；应把读取结果作为循环条件 |
| `in.read(buffer, n)` | 尝试读取 n 字节，返回流引用 | 用 `gcount()` 查询实际读取数，末尾可能不足 n 字节 |
| `out.write(buffer, n)` | 写入 n 字节，返回流引用 | 缓冲区至少含 n 字节，之后仍须检查流状态 |
| `out.flush()` / `out.close()` | 刷新返回流引用，关闭返回 void | 写入失败可能到刷新或关闭才暴露；关闭后检查状态。flush 不等于保证物理落盘 |

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    std::ifstream in(argv[1]);
    if (!in) return 1;
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << '\n';
    }
    if (in.bad() || (in.fail() && !in.eof())) return 1;
}
```

避免 `while (!in.eof())`：文件尾状态通常在一次读取尝试后才设置。二进制文件可用 `std::ios::binary`，不需要手动释放流内部的缓冲区。

## POSIX fd 与本仓库的 RAII

| 接口 | 参数、返回值 | 所有权与错误 |
| --- | --- | --- |
| `::open(path, flags[, mode])` | 路径字符串、打开标志、创建权限；返回 fd，失败为 -1 | 例如 `O_RDONLY`；使用 `O_CREAT` 时提供 mode，实际权限还受 umask 影响；fd 0 也是有效值 |
| `::read(fd, buffer, count)` | 目标缓冲区和最大字节数；返回实际字节数、0 表示 EOF、-1 表示错误 | 可能短读；用有符号 `ssize_t` 保存结果，不能先转为无符号再判断错误 |
| `::write(fd, buffer, count)` | 源缓冲区和字节数；返回实际写入数，-1 表示错误 | 可能短写，完整发送需循环处理剩余部分；按 errno 区分 EINTR 等情况 |
| `::close(fd)` | 返回 0 或 -1 | 释放句柄；不要在析构中盲目重试，避免误关复用的编号 |

`errno`（`<cerrno>`）只在接口报告失败后用于分析该次错误。文件偏移会影响后续读写；非阻塞 fd 还需要处理暂时不可读写的状态。

`UniqueFd::get()`、`release()`、`reset()` 是 **04 练习自定义接口**：get 借用，release 交出关闭责任并使自身为空，reset 关闭原有的不同 fd 并接管新的 fd。这些名称不属于 `std::filesystem`，也不能把练习中 reset 的“相同 fd 时直接返回”规则套用到标准 `unique_ptr::reset`。实际练习错误见 [学习笔记](../learning-notes.md#04--raii-与文件描述符所有权)。
