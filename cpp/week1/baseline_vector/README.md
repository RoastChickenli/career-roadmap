注意：这是三条命令，需要分别执行，每输入一条按一次 Enter：

```bash
cmake -S . -B build
cmake --build build
./build/main
```

它们依次完成：

```text
读取项目配置 → 编译程序 → 运行程序
```

## 第一条：配置项目

```bash
cmake -S . -B build
```

逐项解释：

| 内容      | 含义                         |
| ------- | -------------------------- |
| `cmake` | 启动 CMake 程序                |
| `-S`    | 指定源码目录，S 是 Source 的缩写      |
| `.`     | 当前目录                       |
| `-B`    | 指定构建目录，B 是 Build 的缩写       |
| `build` | 将生成的构建文件和编译结果放在 `build` 目录 |

因此，整条命令的意思是：

> 读取当前目录中的 `CMakeLists.txt`，然后在 `build` 目录中生成编译所需的文件。

### 为什么 `.` 表示当前目录？

Linux 路径中：

| 符号   | 含义       |
| ---- | -------- |
| `.`  | 当前目录     |
| `..` | 上一级目录    |
| `~`  | 当前用户的家目录 |

你现在位于：

```text
~/career-roadmap/cpp/week1/baseline_vector
```

因此：

```bash
cmake -S .
```

就是告诉 CMake：

> 去 `baseline_vector` 当前目录寻找 `CMakeLists.txt`。

### `-B build` 做了什么？

`-B build` 表示把编译过程中产生的文件放到：

```text
baseline_vector/build/
```

如果 `build` 目录不存在，CMake 会自动创建。

执行前：

```text
baseline_vector/
├── CMakeLists.txt
└── main.cpp
```

执行后大致会变成：

```text
baseline_vector/
├── CMakeLists.txt
├── main.cpp
└── build/
    ├── CMakeCache.txt
    ├── CMakeFiles/
    └── Makefile
```

具体生成 `Makefile` 还是 Ninja 文件，取决于系统使用的构建工具。

这一步通常还没有真正编译 `main.cpp`，主要完成：

1. 读取 `CMakeLists.txt`
2. 检查 C++ 编译器
3. 检查编译器是否可用
4. 确定 C++ 标准和编译选项
5. 生成后续编译需要的构建文件

这一步通常称为：

```text
配置阶段（Configure）
```

---

## 第二条：编译项目

```bash
cmake --build build
```

逐项解释：

| 内容        | 含义                  |
| --------- | ------------------- |
| `cmake`   | 启动 CMake            |
| `--build` | 告诉 CMake：现在开始执行编译   |
| `build`   | 使用 `build` 目录里的构建配置 |

整条命令的意思是：

> 根据 `build` 目录中已经生成的构建文件，编译项目。

CMake 会在背后调用：

```text
g++
```

以及 Make 或 Ninja 等构建工具。

假设你的 `CMakeLists.txt` 是：

```cmake
add_executable(main main.cpp)
```

CMake 就知道：

* 需要编译的源文件是 `main.cpp`
* 最终生成的可执行程序叫 `main`

所以：

```bash
cmake --build build
```

在当前这个简单项目中，大致相当于：

```bash
g++ main.cpp -o build/main
```

但真实的编译命令还可能包含：

```text
-std=gnu++17
-I...
-D...
```

这些参数由 CMake 自动管理。

编译成功后，目录大致是：

```text
baseline_vector/
├── CMakeLists.txt
├── main.cpp
└── build/
    ├── CMakeCache.txt
    ├── CMakeFiles/
    ├── Makefile
    └── main
```

其中：

```text
build/main
```

就是生成的可执行程序。

### 为什么不用直接执行 `make`？

你也可能看到这种写法：

```bash
cd build
make
```

它在使用 Makefile 时也可以工作。但是：

```bash
cmake --build build
```

更通用，因为 CMake 会自动判断底层使用的是：

* Make
* Ninja
* Visual Studio
* 其他构建工具

因此建议优先记住：

```bash
cmake --build build
```

---

## 第三条：运行程序

```bash
./build/main
```

这不是 CMake 命令，而是一条普通的 Linux 程序运行命令。

逐项解释：

| 内容      | 含义                        |
| ------- | ------------------------- |
| `.`     | 当前目录                      |
| `/`     | 路径分隔符                     |
| `build` | 进入当前目录下面的 `build` 目录      |
| `/main` | 运行 `build` 目录中的 `main` 文件 |

因此：

```bash
./build/main
```

表示：

> 运行当前目录下 `build` 文件夹中的 `main` 程序。

它对应的完整路径是：

```text
~/career-roadmap/cpp/week1/baseline_vector/build/main
```

### 为什么不能只输入 `main`？

Linux 默认不会从当前目录搜索程序。因此通常需要明确写出路径：

```bash
./build/main
```

这里前面的 `./` 表示：

> 从当前目录开始寻找。

如果你先进入 `build`：

```bash
cd build
```

那么就可以运行：

```bash
./main
```

---

## 三条命令的完整关系

你的 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.16)

project(baseline_vector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
```

然后执行：

```bash
cmake -S . -B build
```

含义：

> 分析项目并生成构建配置。

接着执行：

```bash
cmake --build build
```

含义：

> 根据构建配置调用编译器，生成 `build/main`。

最后执行：

```bash
./build/main
```

含义：

> 运行生成的程序。

可以把它理解为：

```text
CMakeLists.txt
      ↓
cmake -S . -B build
      ↓
生成编译规则
      ↓
cmake --build build
      ↓
生成 build/main
      ↓
./build/main
      ↓
程序开始运行
```

以后只修改了 `main.cpp`，通常不需要重新配置，直接执行：

```bash
cmake --build build
./build/main
```

第一次构建项目时，则完整执行三条命令。
