/*
1.new Task(...)做了什么，返回了什么？

返回一个指向具体实列的内存地址
① 在堆区分配内存并调用构造函数；② 返回这块内存的地址。
2.普通指针变量和它指向的Task对象分别存放在哪里？

指针变量 task：存放在栈区（Stack）。因为它是一个局部变量。
Task 对象：存放在堆区（Heap）。因为它是用 new 动态分配的。
3.delete task依次完成了哪些事情？
//将task指向的地址内容清除

调用 Task 的析构函数 ~Task()（执行清理逻辑，比如打印“已销毁”）。
释放堆区的内存，内存交还给 C++ 内存分配器
它不会清除指针变量 task 本身，task 还在栈上，只是变成了野指针
4.什么是内存泄漏？

//程序已经运行完了，还可以通过指针找到原来存放在内存中的内容。(错误)

在堆区用 new 分配了内存，但忘记用 delete 释放，并且丢失了指向它的指针。
这块内存就永远无法被回收，直到程序结束
5.为什么unique_ptr不能直接复制？

//因为具有唯一性（我也不清楚）
独占所有权。如果允许复制（auto b = a;），那么 a 和 b 就都指向同一块内存。
当它们离开作用域时，会各自调用一次 delete，导致同一块内存被释放两次（Double Free），程序直接崩溃。
6.std::make_unique()有什么作用？

创建unique_ptr智能指针。

7.std::move(owner1)执行后，owner1和owner2分别是什么状态？

owner1指向nullptr失去了对内存的控制权。
owner2指向接管了那块堆内存的地址，成为新的唯一主人。
8.为什么不能手动delete智能指针管理的对象？
因为智能指针的析构函数会自动 delete 它管理的对象。
如果你手动 delete 了，等智能指针离开作用域时又会 delete 一次，导致 Double Free（双重释放），程序直接崩溃。
9.本题怎样体现了RAII？
RAII 全称是 Resource Acquisition Is Initialization（资源获取即初始化）。
核心思想：把资源（比如堆内存）的生命周期，绑定到一个局部对象（比如 unique_ptr）的生命周期上。
在本题中的体现：在 unique_pointer_demo 中，Task 对象的生命周期被完美绑定到了 unique_ptr 这个局部变量上。
只要 unique_ptr 离开作用域自动销毁，它管理的 Task 就必然被自动释放，彻底杜绝了内存泄漏。这就是 RAII 的威力。




栈区（Stack）—— 自动管理的“临时工作台”
它是什么：它是系统自动管理的一块内存，专门用来存放局部变量（比如你在函数里写的 int a = 10; 或者 Task task1(...)）。
特点：
自动分配，自动回收：当你进入一个函数（比如 main 函数），系统会自动在栈上给你划一块地。当函数结束（return），系统会自动把这块地收走。你完全不需要操心。
速度极快：因为系统已经规划好了怎么管理它，所以存取速度非常快。
空间有限：栈区的大小通常是固定的（比如几 MB），不能无限塞东西。
2. 堆区（Heap）—— 手动管理的“大型仓库”
它是什么：它是程序员自己管理的一块内存，专门用来存放动态分配的对象（比如你用 new Task(...) 创建的对象）。
特点：
手动分配，手动回收：你必须主动用 new 去申请一块地，用完后必须主动用 delete 去归还。如果你忘了 delete，这块地就永远被占着（内存泄漏）。
速度较慢：因为每次 new 和 delete 都需要向操作系统去申请和归还内存，开销比较大。
空间巨大：堆区通常很大，只要你的电脑内存够，就可以一直 new 下去。
*/

/*
三种写法的本质
Task task1(1, "compile", 3);

task1本身就是对象。

Task* task2 = new Task(2, "test", 1);

task2只是地址，真正的对象在另一块动态内存中，需要手动：

delete task2;
auto task3 = std::make_unique<Task>(3, "download", 2);

task3是自动管理对象地址的智能指针，不需要手动 delete。

*/



#include<iostream>
#include<string>

#include<memory> // std::unique_ptr, std::make_unique
#include<utility>  // std::move

class Task{
public:
//构造函数
Task(int id,const std::string& type,int priority);


//析构函数
~Task();

//输出函数
void print() const;

//获取优先级函数
int get_priority() const;



private:
int id_;
std::string type_;
int priority_;

};
////////构造列表
//构造函数实现
Task::Task(int id,const std::string& type,int priority):id_(id),type_(type),priority_(priority){
std::cout<<"[构造] 任务"<<id_<<"已创建"<<std::endl;
}


//析构函数实现
//~Task(){ }-------------------------------错误
Task::~Task(){
    std::cout<<"[析构] 任务"<<id_<<"已销毁"<<std::endl;
}
//输出函数实现
void Task::print() const {
  std::cout << "任务编号：" << id_ << std::endl;
    std::cout << "任务类型：" << type_ << std::endl;
   std::cout << "任务优先级：" << priority_ << std::endl;
}

//优先级函数实现
int Task::get_priority() const {
    return priority_;
}







//1.普通指针实验nullptr 也是由 null（空）和 ptr（pointer）组合

void raw_pointer_demo(){
Task* task1=new Task(1,"compile",3);//task本身不是对象，指向对象的指针
task1->print();//-------------------指针怎么使用方法（直接方法调用不同）--task->print() 其实是 (*task).print() 的简写。
delete task1;//必须手动释放   调用了析构函数，释放了内存）。
task1=nullptr;//房子没了！但是，task 手里还攥着那个旧门牌号.在 C++ 里叫“野指针”。如果你这时候再用 task->print()，系统就会崩溃.
}

// 2. unique_ptr 实验
/*
std::make_unique<Task>(2, "test", 1)
std::make_unique：这是一个“工厂函数”，专门用来生产智能指针。
<Task>：告诉工厂，你要生产的对象是 Task 类型的。
(2, "test", 1)：这是传给 Task 构造函数的参数。
它做了什么：它在内存里 new 了一个 Task(2, "test", 1) 对象，然后立刻把这个对象的地址，塞进了一个 unique_ptr（独占智能指针）里。
auto task = ...
auto：这是 C++ 的“自动类型推导”。因为 make_unique 返回的类型非常长（std::unique_ptr<Task>），写起来太麻烦，所以用 auto 让编译器自己去猜。
task：这就是你给这个智能指针起的名字。
*/
void unique_pointer_demo(){
//std::unique_ptr<Task> task2 = ...   auto 是 C++ 的“自动类型推导”
  auto task2= std::make_unique<Task>(2, "test", 1);

  task2->print();//task->print() 其实是 (*task).print() 的简写。
    // 绝对不能手动 delete
    // 函数结束时，task 自动析构，并自动 delete 内部的 Task 对象
}
//3.所有权转移
void ownership_transfer_demo(){
    //auto task3_1=std::unique_ptr<Task>(3, "deploy", 2);//----------------错误  如何声明unique_ptr；

auto task3_1=std::make_unique<Task>(3, "deploy", 2);
 // 将所有权从 task3_1 转移给 task3_2
 std::unique_ptr<Task> task3_2=std::move(task3_1);
 if(task3_1==nullptr){
    std::cout<<"task3_1已经不再拥有任务3"<<std::endl;
 }
// 通过 task3_2 正常使用
task3_2->print();
}



//主函数
int main(){
//Task::Task task1();----------------------错误：使用类的时候不需要加Task::   实现内部方法的时候需要添加
   std::cout << "===== 普通指针实验 =====" << std::endl;

  raw_pointer_demo();
std::cout << "===== unique_ptr实验 =====" << std::endl;
unique_pointer_demo();


std::cout << "===== 所有权转移实验 =====" << std::endl;

ownership_transfer_demo();
std::cout << "===== 程序结束 =====" << std::endl;
return 0;
}