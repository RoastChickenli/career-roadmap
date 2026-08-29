"""
1.json.load(file)返回的数据类型是什么？

数据类型与json文件中的内容有关
本提中json存储的内容对应为python中的列表

2.return []中的[]表示什么？为什么出错时返回它？
[]表示空列表
可以在后面的程序用使用if语句进行判断

3.sorted()与列表的.sort()有什么区别？
列表的.sort()  列表专属方法，改变原列表顺序
sorted() 适用于所有可迭代对象（如列表、元组、字典的键等） 返回一个全新的排好序的列表，原列表不变


4.reverse=True有什么作用？
reverse=Fase从小到大
reverse=True从大到小

5.if __name__ == "__main__":有什么作用？
开关作用
如果要是直接运行此文件main.py
判断语句成立，直接运行main()
如果要是再别的文件中import main
判断语句不成立，不会运行main()



编写main.py，完成以下功能：

从tasks.json读取任务；
按priority从高到低排序；
输出排序后的全部任务；
文件不存在时输出明确提示；
JSON格式错误时输出明确提示；
文件中没有任务时输出明确提示；
程序不能直接出现一大段连续执行代码，需要拆分成函数。

"""

"""
| 模式 | 英文全称 | 核心行为 | 文件不存在时 | 文件已存在时 | 适用场景 |
| `'r'` | Read (默认) | 只读。光标在文件开头。 | ❌ 报错 (`FileNotFoundError`) | 正常读取 | 读取配置文件、JSON、TXT 等 |
| `'w'` | Write | 写入。光标在文件开头。 | ✅ 自动创建新文件 | ⚠️ 直接清空原内容（非常危险） | 重新生成日志、导出全新数据 |
| `'a'` | Append | 追加。光标在文件末尾。 | ✅ 自动创建新文件 | 保留原内容，在末尾继续写 | 记录运行日志、追加新数据 |
| `'x'` | Create | 排他创建。仅用于新建文件。 | ✅ 自动创建新文件 | ❌ 报错 (`FileExistsError`) | 确保不会意外覆盖重要文件 |


| 模式 | 英文全称 | 核心行为 | 适用场景 |
| `'r+'` | Read + Write | 可读可写。光标在开头，不清空原内容。 | 修改现有文件中的部分内容（需配合 `seek()` 移动光标） |
| `'w+'` | Write + Read | 可写可读。光标在开头，会清空原内容。 | 需要完全重写文件，但写完后又需要立刻读取它 |
| `'a+'` | Append + Read | 追加并读取。写入时光标强制在末尾，读取时光标可移动。 | 既要往日志末尾写数据，又要回头读取历史日志 |
"""
#open("tasks.json", "r", encoding="utf-8")   open用法
import json
#with open("tasks.json","r",encoding="utf-8") as file:
#    tasks=json.load(file)
#python 方法使用

"""
python if的使用方法详情readme.md
"""



def load_tasks(filename):
    try:    #--------------------------------------------------------try...except  用法，注意代码的层级
       with open(filename,"r",encoding="UTF-8") as file:
            tasks=json.load(file)
            return tasks
    except FileNotFoundError:
        print("错误：找不到文件",filename)
        return None
    except json.JSONDecodeError:
        print("JSON文件格式错误：")
        return None

"""
sorted() 是 Python 的内置排序函数。它不会改变原来的 tasks 列表，而是返回一个全新的、排好序的列表。括号里的三个参数分别决定了“对谁排序”、“按什么标准排序”和“怎么排”。
参数一：tasks（要排序的对象）
告诉 Python：“请对 tasks 这个列表进行排序。”

参数二：key=lambda task: task["priority"]（排序的标准/规则）⭐️最难点
这是整个函数中最难懂的部分。key 参数的作用是：告诉 Python 在比较两个任务时，到底该看任务的哪个部分？
lambda task:：这是 Python 的“匿名函数”（可以理解为一种极简的函数写法）。task 是临时变量，代表列表中的每一个任务字典。
task["priority"]：这是排序的依据。意思是：“请提取出每个任务字典里 priority 对应的值，用这个值来进行大小比较。”

参数三：reverse=True（排序的方向）
reverse=False（默认）：升序（从小到大，比如 1, 2, 3）。
reverse=True：降序（从大到小）。在你的需求中，优先级数字越大越重要，所以用 True 让它从高到低排。

"""

def sort_tasks(tasks):
    sorted_tasks=sorted(
        tasks,
        key=lambda task:task["priority"],
        reverse=True
    )
    return sorted_tasks

def print_tasks(tasks):
    for task in tasks:
        print(task)
        print("__________________")

def main():
    filename="tasks.json"
    tasks=load_tasks(filename)
    #if tasks is None-----------------------------错误判断语句不会用

    if tasks is None:
       return
    """
在 Python 的 if 语句中，并不是只有 True 和 False 才能被判断，Python 会自动把很多数据转换成布尔值：
🚫 这些值会被认为是“假”（False）：
空列表：[]
空字典：{}
空字符串：""
数字 0：0 或 0.0
None：代表什么都没有
✅ 这些值会被认为是“真”（True）：
有内容的列表：[1, 2, 3]
有内容的字典：{"name": "Alice"}
有内容的字符串："hello"
非零数字：1, -5, 3.14
结合你的代码：
如果 tasks = []（空列表），Python 会认为它是“假”的。加上 not 取反后，not [] 就变成了 True，于是 if 条件成立，程序就会打印“提示：文件中没有任务”。
    
    """
    if not tasks:
        print("提示：文件中没有任务")
        return
    print("任务读取成功")


    result=sort_tasks(tasks)
    print_tasks(result)

if __name__=="__main__":
    main()
        