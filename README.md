# CMake构建目录
**/build/

# 手动生成的可执行文件
**/main

# Python缓存
**/__pycache__/
*.pyc

# Python虚拟环境
.venv/
venv/

# GDB历史
.gdb_history


每条规则的含义
忽略所有 build目录
**/build/

其中：

**  表示任意层级目录
/   结尾表示它是目录

它会匹配：

cpp/week1/baseline_vector/build/
cpp/week2/vector_statistics/build/
linux/week1/linux_process/build/
忽略名称恰好为 main的文件
**/main

会忽略：

cpp/week2/vector_statistics/main
linux/week2/multi_process/main
linux/week2/waitpid_process/main

但不会忽略：

main.cpp
main.c
main.py

因为这些文件的名称不是恰好的 main。

忽略Python缓存
**/__pycache__/
*.pyc

忽略Python运行时自动产生的缓存文件。

忽略虚拟环境
.venv/
venv/

Python虚拟环境可能包含大量依赖文件，不应该提交。

忽略GDB历史
.gdb_history

它记录你输入过的GDB命令，属于本机调试历史，不属于项目源代码。