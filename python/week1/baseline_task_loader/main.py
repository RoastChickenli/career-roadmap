#从json文件中读取内容，需要import json
import json
print("任务加载启动")
#什么是字典{}  整数行不加""
task={
   "id":1,
   "type":"compile",
   "priority":3   
}
print(task)
print("任务编号：",task["id"])
print("任务类型：",task["type"])
print("任务优先级：",task["priority"])
#什么是列表[]

taskb=[
    {
        "id":1,
        "type":"compile",
        "priority":3
    },
    {
        "id":2,
        "type":"test",
        "priority":1       
    },
    {
        "id":3,
        "type":"deploy",
        "priority":2    
    }
]
print(taskb)
print("任务数量：",len(taskb))    #----------------------------列表！！！！！[]方法  长度len()

print("第一个任务：",taskb[0])#----------------------------链表[]方法  取数据元素

#错误  结合的时候需要遵守字典中的原来数据类型  taskb[0][id]  ---->taskb[0]["id"]
#print("第一个任务编号：",taskb[0][id])#---------------------------与字典结合使用
print("第一个任务编号：",taskb[0]["id"])
print("第一个任务类型：", taskb[0]["type"])
print("第一个任务优先级：", taskb[0]["priority"])


print("输出任务：")

"""
第一次循环：

current_task = taskb[0]

此时：

current_task = {
    "id": 1,
    "type": "compile",
    "priority": 3
}

第二次循环：

current_task = taskb[1]
"""

for current_task in taskb:   #---------------------------------与c、c++不同
    print(current_task)


print("从json中读出数据")
#------------------------------------来了，重点。从来没有遇到过python
"""
with ... as file是什么意思？

完整代码是：

with open("tasks.json", "r", encoding="utf-8") as file:

可以拆成两部分理解。

打开文件：

open("tasks.json", "r", encoding="utf-8")

将打开的文件临时命名为：

file

因此：

as file

可以理解为：

把已经打开的文件交给变量 file管理。

file代表已经打开的 tasks.json，它不是文件名字符串，而是一个可以读取的文件对象。
"""
with open("tasks.json","r", encoding="utf-8") as file:
    tasks=json.load(file)
print("json文件读取成功")
print("任务数量：",len(tasks))
for current_task in tasks:
    print("任务编号为：",current_task["id"])
    print("任务类型：", current_task["type"])
    print("任务优先级：", current_task["priority"])
    print("--------------------")
"""
son.load(file)做了什么？
tasks = json.load(file)

可以分成三步：

从 file中读取JSON内容；
检查并解析JSON格式；
将解析结果转换成Python数据并保存到 tasks。

转换关系是：

tasks.json中的JSON数组
             ↓ json.load()
Python列表tasks
             ↓
列表中的每个JSON对象变成Python字典
"""