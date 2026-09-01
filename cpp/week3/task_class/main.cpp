/*
1.class中的private有什么作用？
封装与保护。
private 下面的成员（变量或函数），外部代码无法直接访问。
防止数据被意外篡改。外部只能通过提供的 public 函数（如 get_priority()）来安全地读取数据。

2.构造函数什么时候自动调用？
对象被创建的瞬间。
3.析构函数什么时候自动调用？
对象即将被销毁的瞬间
4.为什么task2离开内部代码块时就会被销毁？
task2 是在 { ... } 这个内部代码块里定义的。一旦程序执行到右花括号 }，
就意味着离开了这个代码块。C++ 规定，局部对象在离开其作用域时，必须被立即销毁，以释放内存。所以 task2 的析构函数会在这里立刻触发。
5.为什么print()后面要写const？
void print() const; 末尾的 const 告诉编译器：“这个函数绝对不会修改对象的任何成员变量
有一个“只读”的任务对象（比如 const Task t1(...)），你依然可以调用 t1.print()。
如果没有这个 const，编译器会禁止你对只读对象调用 print()，因为它担心 print() 会偷偷修改数据。
6.构造函数初始化列表与在函数体中赋值有什么区别？
初始化列表 (: id_(id))：对象在诞生的同时，直接用参数完成初始化。这是一步到位。
函数体赋值 (id_ = id;)：对象诞生时先默认构造（比如 std::string 先创建一个空字符串），然后在函数体里再赋值覆盖。这是两步走。
结论：对于 std::string 这类复杂类型，初始化列表效率更高（少一次默认构造和赋值）。
对于 const 成员或引用成员，必须用初始化列表，不能在函数体赋值。
7.同一作用域中的多个对象通常按照什么顺序析构？
后进先出（LIFO），与构造顺序相反
定义一个Task类，用来描述任务。

它包含三个私有成员：

int id_;
std::string type_;
int priority_;

必须实现：

class Task {
public:
    Task(
        int id,
        const std::string& type,
        int priority
    );

    ~Task();

    void print() const;

    int get_priority() const;

private:
    int id_;
    std::string type_;
    int priority_;
};

不能修改这些函数声明。
二、构造函数要求

创建对象时，构造函数保存参数，并输出：

[构造] 任务1已创建

要求使用构造函数初始化列表：

Task::Task(...)
    : id_(...),
      type_(...),
      priority_(...)
{
    //输出 
}

不能先使用默认值创建成员，再在函数体中逐个赋值。

三、析构函数要求

对象销毁时，析构函数输出：

[析构] 任务1已销毁

本题通过输出观察对象何时被自动销毁。

四、成员函数要求

print()输出：

任务编号：1
任务类型：compile
任务优先级：3

get_priority()返回：

priority_

这两个函数都必须带有末尾的const：

void print() const;
int get_priority() const;

*/
#include<iostream>
#include<string>
//创建task类

class Task{

//Task(...);---------------------------------构造函数
//~Task();-------------------------析构函数
/*
构造函数（Constructor）：负责“生”。当你创建一个对象时，它会自动运行。
就像工厂流水线，零件一上来，构造函数就负责把编号、类型、优先级这些初始数据塞进去。

析构函数（Destructor）：负责“死”。当对象不再被需要、即将被系统回收销毁时，它会自动运行。
就像垃圾回收员，在对象消失前做最后的清理或告别（比如题目要求的打印“已销毁”）。
*/
public:
//构造函数
    Task(
     //   int id_;---------------------------------错误;与下划线_
      //  std::string tpye_;
       // int priority_;
        int id,
        //std::string type,----------------------------------------------------------错误，内外必须一致
        const std::string& type_,//和std::string type_;不是同类型
        int priority

    );
//析构函数
~Task();
//打印函数
void print() const;
int get_priority() const;
/*
这个 const 不是修饰 void 的，也不是修饰 print 的，它是修饰整个函数的“行为”的。
你可以把它理解成这个函数的一个“人品保证”或“安全承诺”。
没有 const 的 print()：
系统会想：“这个函数可能会修改任务卡片里的数据，比如把编号改了，或者把优先级调高了。我得小心点。”
有 const 的 print() const：
系统会想：“哦，这个函数带了 const 保证，它绝对不会修改任务卡片里的任何数据。它只是‘看看’，非常安全！”
*/
  /*  
    private int id_;-----------------------------------------错误c++私有共有变量的声明不同
    private string type_;
    private priority_;
  */ 
private:
    int id_;
    std::string type_;
    int priority_;

};//---------------------------------------------------------错误：类结束后加;

//构造函数实现

Task::Task(int id,const std::string& type,int priority):id_(id),type_(type),priority_(priority){//------------------初始化列表
std::cout<<"[构造] 任务"<<id_<<"已创建" << std::endl;
}
// 析构函数实现
Task::~Task(){
    std::cout<<"[析构] 任务"<<id_<< "已销毁" << std::endl;//-------------------------------使用的参数全部带下划线id_
}
// print() 函数实现
void Task::print() const {//----------------------------------------函数实现时，void位置 
   std::cout << "任务编号：" << id_ << std::endl;
    std::cout << "任务类型：" << type_ << std::endl;
   std::cout << "任务优先级：" << priority_ << std::endl;
}
//get_priority()函数实现
int Task::get_priority() const{
    return priority_;
}

int main(){

/*
创建任务1；
打印任务1；
进入一个内部代码块；
在内部代码块中创建任务2；
打印任务2；
离开内部代码块；
输出“已经离开内部代码块”；
再创建并打印任务3；
结束程序。
*/

Task task1(1,"compile",3);
task1.print();

    {
        Task task2(2,"test",1);
        task2.print();
    }

    std::cout << "已经离开内部代码块\n";

    Task task3(3,"deploy",2);
    task3.print();

    
    return 0;
}