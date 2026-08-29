创建 tasks.json任务文件

目前任务直接写在 main.py中：

taskb = [
    ...
]

作业要求从JSON文件读取，所以现在把任务数据独立保存到 tasks.json。

1. 创建 tasks.json

确认位于项目目录：

cd ~/career-roadmap/python/week1/baseline_task_loader

创建文件：

nano tasks.json

写入：

[
    {
        "id": 1,
        "type": "compile",
        "priority": 3
    },
    {
        "id": 2,
        "type": "test",
        "priority": 1
    },
    {
        "id": 3,
        "type": "deploy",
        "priority": 2
    }
]

保存并退出：

Ctrl + O
按 Enter
Ctrl + X
2. JSON文件的数据结构

最外层是：

[
]

表示一组任务。在JSON中称为数组，对应Python的列表。

每一个任务是：

{
    "id": 1,
    "type": "compile",
    "priority": 3
}

在JSON中称为对象，对应Python的字典。

对应关系是：

JSON	Python
数组 []	列表 []
对象 {}	字典 {}
字符串 "compile"	字符串 "compile"
数字 3	整数 3

整体结构是：

JSON数组
├── 第一个任务对象
├── 第二个任务对象
└── 第三个任务对象
3. JSON和Python代码的区别

在Python中可以写变量：

taskb = [
    ...
]

但是JSON只保存数据，不能写：

taskb = [
    ...
]

taskb =必须删除。因此JSON文件直接从 [开始。

JSON文件也不能写Python注释：

# 这是任务列表

这种写法是错误的。

4. JSON必须使用双引号

正确：

"type": "compile"

错误：

'type': 'compile'

虽然Python字典允许单引号，但是标准JSON要求字符串和键使用双引号。

整数不需要引号：

"priority": 3

如果写成：

"priority": "3"

它就会变成字符串，不是整数，不适合后面进行数值排序。

5. 最后一项后面不能多写逗号

正确：

{
    "id": 3,
    "type": "deploy",
    "priority": 2
}

错误：

{
    "id": 3,
    "type": "deploy",
    "priority": 2,
}

JSON中的最后一个属性后面不能保留逗号。

最后一个任务对象后面也不能多写逗号：

[
    {"id": 1},
    {"id": 2}
]