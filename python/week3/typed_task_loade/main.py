"""
类型注解就是给变量和函数贴上的“说明书标签”，告诉别人它们应该是什么类型，但 Python 运行时并不会真的去检查它。
age: int = 25
name: str = "Alice"
类型注解函数

def add(x: int, y: int) -> int:
    return x + y

1.类型注解会不会在程序运行时自动阻止错误类型？
不会。 类型注解（比如 id: int）纯粹是写给开发者和编辑器看的“说明书”。
Python 解释器在运行时完全忽略它，不会自动拦截错误类型。要拦截，必须自己写 if 判断。

2.object | None表示什么？
函数返回的结果有两种可能object或者None

3.list[Task]表示什么？
一个元素为Task的列表

4.dataclass帮我们自动生成了哪些常用功能？
自动生成了 __init__（构造函数，不用自己写初始化代码）、__repr__（打印对象时显示友好内容，而不是内存地址）和 __eq__（对象比较）。

1. __init__（构造函数）
作用： 当你创建一个新对象时，自动帮你把数据存进去。
class Task:----------------------------------------------------------无@dataclass
    def __init__(self, id: int, type: str, priority: int):
        self.id = id
        self.type = type
        self.priority = priority

@dataclass
class Task:-----------------------------------------@dataclass
    id: int
    type: str
    priority: int

 2.__repr__（打印友好显示）
print(my_task)----------------------------------------------------------无@dataclass
# 输出: <__main__.Task object at 0x7f8b1c2d3e50> （一堆看不懂的内存地址）
print(my_task)----------------------------------------------------------@dataclass
# 输出: Task(id=1, type='compile', priority=3) （清晰明了！）

3. __eq__（对象比较）
task1 = Task(1, "compile", 3)----------------------------------------------------------无@dataclass
task2 = Task(1, "compile", 3)
print(task1 == task2)
# 输出: False （默认比较的是内存地址，两个对象地址不同，所以不等）
print(task1 == task2)----------------------------------------------------------@dataclass
# 输出: True （自动比较 id、type、priority 都相同，所以相等）

5.为什么JSON解析结果不能直接认为是list[Task]？
因为json的内容不确定
json.load() 只能解析成 Python 基础类型（dict、list、str 等），它不认识你自定义的 Task 类。

6.为什么需要先判断JSON最外层是不是列表？
因为后续代码要用 for 循环遍历数据。如果最外层是字典，直接遍历会出错或逻辑错乱。提前校验能保证后续代码安全。

7.isinstance(True, int)为什么是True？
因为在 Python 中，bool 是 int 的子类。True 本质上就是整数 1，False 就是整数 0。

8.本题为什么使用type(value) is int？
true默认等于1
用 type() is int 能严格排除 True/False 这种“伪装”成整数的布尔值。

9.Task | None表示这个函数可能返回哪两种结果？
Task类或者None

10.task.priority和task["priority"]分别适用于什么对象？
task.priority 适用于对象（Task 实例），通过属性访问；task["priority"] 适用于字典（JSON 解析出来的原始数据），通过键名访问。

11.为什么一条任务验证失败后，不应该让整个程序退出？
因为外部数据往往是“脏”的。如果一条坏数据就让程序崩溃，其他好数据也无法处理。健壮的程序应该跳过坏数据，继续处理剩下的，并给出明确提示。

12.类型注解和运行时数据校验分别解决什么问题？
类型注解：解决开发阶段的问题。帮助 IDE 提供智能提示，帮助开发者理清逻辑。
运行时校验：解决生产环境的问题。外部输入永远不可信，必须在程序执行时用 if 拦截非法数据，防止崩溃。
"""
import json
from dataclasses import dataclass

@dataclass
class Task:
    id:int
    type:str
    priority:int

#str 是 Python 内置的字符串类型，而不是你传入的参数名。你应该用参数名 filename
def load_json(filename:str) ->object | None:
    """负责打开和解析JSON文件，处理文件不存在和语法错误。"""
    try:
        with open(filename,"r",encoding="UTF-8") as file:
            tasks=json.load(file)#------------------------------必须要缩进with下面的需要缩进
        return tasks
    except FileNotFoundError:
        print("错误：找不到文件",filename)
        return None
    except json.JSONDecodeError:
        print("JSON文件格式错误：")
        return None
def validate_task(raw_task:object,index:int)->object|None:
    """验证单条原始数据是否合法，不合法返回 None 并打印原因。"""
    """函数用法
    isinstance(对象, 类型)
    如果“对象”确实是“类型”（或它的子类），它就返回 True。
    如果不是，就返回 False。

    例子：isinstance(raw_task, dict)-----------dict字典类型{"id": 1, "type": "test"}
    如果 raw_task 是 {"id": 1, "type": "test"}，结果就是 True。
    如果 raw_task 是 "hello" 或者 123，结果就是 False。
    name = "Alice"
    print(isinstance(name, str))  # 输出: True
    age = 25
    print(isinstance(age, int))   # 输出: True
    numbers = [1, 2, 3]
    print(isinstance(numbers, list))  # 输出: True
    """

    if not isinstance(raw_task,dict):
        print(f"第{index}项任务无效：数据格式错误，必须是字典")
        return None
    # 1. 检查字段是否存在
    required_fields=["id","type","priority"]
    for field in required_fields:
        if field not in raw_task:
            print(f"第{index}项任务无效：缺少{field}字段")
            return None
    # 2. 验证 id (必须是正整数，排除 True/False)
    """
    因为前面已经确认了 raw_task 是一个字典，所以这里用 ["id"] 从字典里把 id 对应的值取出来
    raw_task["id"]
    
    第一个条件：必须是“纯正”的整数
    type(raw_task["id"]) is not int
    type(raw_task["id"])：获取 id 值的真实类型。
    is not int：判断这个类型是不是不等于 int
    ---------------------------------------------------------------------------------不会的地方
    为什么要用 type(...) is not int 而不是 isinstance(..., int)？
    因为在 Python 中，bool（布尔类型）是 int（整数类型）的子类。True 本质上就是 1，False 就是 0。
    如果用 isinstance(True, int)，结果会是 True，这就会让 True 蒙混过关。
    用 type(...) is not int 就能严格区分，把 True 和 False 这种“伪装者”挡在门外。
    """

    if type(raw_task["id"]) is not int or raw_task["id"]<= 0:
        print(f"第{index}项任务无效：id必须是正整数")
        return None
    # 3. 验证 type (必须是非空字符串)
    """
    不能是空字符串
    not raw_task["type"]
    """
    if not isinstance(raw_task["type"],str) or not raw_task["type"]:
        print(f"第{index}项任务无效：type不能为空")
        return None   
    # 4. 验证 priority (必须是 1~5 的整数，排除 True/False)
    #if type(raw_task["priority"]) is not int or raw_task["priority"]>5 or raw_task["priority"]<=0:
    if type(raw_task["priority"]) is not int or not (1<=raw_task["priority"]<=5):
        print(f"第{index}项任务无效：priority必须在1到5之间")
        return None
    return Task(
        id=raw_task["id"],
        type=raw_task["type"],
        priority=raw_task["priority"]
    )


def parse_tasks(data:object)->list[Task]:
    """解析整个 JSON 数据，过滤掉无效任务。"""
    if not isinstance(data,list):
        print("错误：JSON最外层必须是列表")
        return[]
    """
    enumerate()函数使用

    for index, raw_task in enumerate(data, start=1):
    
    data：这是你要遍历的原始数据（比如从 JSON 读出来的一个包含多个字典的列表）。

    enumerate(data, start=1)：这个函数的作用是，在遍历 data 的同时，自动给每一项加上编号。

    start=1：告诉 Python，编号从 1 开始（如果不写，默认从 0 开始）。

    index, raw_task：这是“解包”操作。enumerate 每次会返回两个东西：编号 和 数据本身。
    index：自动接收当前的编号（1, 2, 3...）。
    raw_task：自动接收当前遍历到的那条原始数据。
    """
    valid_tasks:list[Task]=[]
    """
    valid_tasks: list[Task]：这是给这个空盒子贴上的“标签”（类型注解）。
    list 表示它是一个列表。
    [Task] 表示这个列表里只能装 Task 类型的对象。
    """
    for index,raw_task in enumerate(data,start=1):
        task=validate_task(raw_task,index)
        if task is not None:
            valid_tasks.append(task)
    return valid_tasks
    """
    sorted()用法
    sorted(tasks,key=lambda task:task.priority,reverse=True)
    第一个参数：tasks（要排队的队伍）
    第二个参数：key=lambda task: task.priority（排队的规则）
    key=：告诉 sorted 函数：“不要按默认规则（比如名字或编号）排序，请按照我指定的规则来排。”

    lambda task: task.priority：这是一个“匿名函数”（lambda 表达式），你可以把它理解为一个临时的、一次性的提取器。
    task：代表列表 tasks 中的每一个 Task 对象。
    :：冒号后面是提取规则。
    task.priority：告诉提取器：“请把这个 Task 对象的 priority（优先级）属性拿出来。”

    
    lambda 表达式
    lambda 参数: 表达式
    lambda：关键字，告诉 Python “我要创建一个匿名函数”。
    参数：函数的输入（比如这里的 task）。
    :：分隔符。
    表达式：函数的输出（比如这里的 task.priority）。注意：它只能写一行，不能写复杂的 if-else 或循环。
    
    lambda task:task.priority
    翻译：
    def get_priority(task):
        return task.priority
    """
def sort_tasks(tasks:list[Task])->list[Task]:
    """按照优先级从高到低排序。"""
    #return sorted(tasks,key=lambda tasks:Task.priority,reverse=True)------错误task与tasks弄混还是不会使用函数
    return sorted(tasks,key=lambda task:task.priority,reverse=True)

def print_tasks(tasks:list[Task])->None:
    """格式化输出任务列表。"""
    #not tasks ====list[Task]=[]
    if not tasks:
        print("没有可执行的合法任务")
        return
    print(f"有效任务数量：{len(tasks)}")
    for task in tasks:
        print(f"任务编号：{task.id}")
        print(f"任务类型：{task.type}")
        print(f"任务优先级：{task.priority}")
        print("-" * 20)

def main()->None:
    data=load_json("tasks.json")
    if data is None:
        return
    tasks=parse_tasks(data)
    sorted_tasks= sort_tasks(tasks)
    print_tasks(sorted_tasks)

if __name__=="__main__":
    main()


