/*

完成代码后，请一并回答：

1.为什么remove_duplicates()使用const std::vector<int>&？
使用const std::vector<int>&
不进行原数vector的复制（资源节省）且不改变原vector(不修改原数据)

2.为什么sort_numbers()不能使用const std::vector<int>&？
sort_numbers()是无返回值的方法
排序需要改变原容器中数据的位置不能加const

3.为什么计算平均值时需要进行类型转换？
因为int只保留整数部分损失精度
4.为什么调用front()和back()之前必须检查vector是否为空？
front()与back()底层是通过指针实现的，如果vector为空。则*begin()与*end()-1为无效地址


第二周  GDB使用问题：

1.为什么要使用-DCMAKE_BUILD_TYPE=Debug？
cmake 会告诉编译器保留源代码的行号、变量名和函数名
这样GDB 才能把内存中的数据对应 到源代码的行数与变量



2.break main做了什么？
在main处设置断点
程序运行到main函数时候停止

3.run < input.txt中的<有什么作用？
输入重定向符号
将input.txt做为std::cin的输入源---重要（读取完文件后，自动生成EOF文件结束标志）
finish 的作用是继续全速运行，直到当前函数执行完毕并返回到上一层。


4.next和step有什么区别？
next：一步一步执行。----遇到函数调用时，会把整个函数当成一个黑盒一口气执行完，不进入内部
step：也是逐步执行，但遇到函数调用时会钻进函数内部，停在函数第一行。


5.finish有什么作用？
finish 的作用是继续全速运行，直到当前函数执行完毕并返回到上一层
当你用 step 钻进了一个函数内部，发现里面没问题，不想再一行行 next 了，就输入 finish。GDB 会一口气跑完当前函数
6.backtrace显示的是什么？
显示的是函数的调用路径（函数调用栈）





*/
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<unordered_set>
/*题目：
编写一个C++程序：

从终端持续读取整数；
使用Ctrl+D结束输入；
保持原有顺序去除重复数字；
将去重结果从小到大排序；
输出最小值、最大值和平均值；
每组输出之间有清楚的空格和换行。*/

/*
参数形式	是否复制	能否修改原数据
std::vector<int> input	是	否
std::vector<int>& input	否	是
const std::vector<int>& input	否	否
*/

//保持原有顺序去除数字
//std::vector<int> remove_duplicates(std::vector<int> input){---------------------第一周参数优化
std::vector<int> remove_duplicates(const std::vector<int> &input){
    std:: unordered_set<int> seen;
    std::vector<int> result;
    for(int num :input){
       if(seen.count(num)==0){
        seen.insert(num);
        result.push_back(num);
       }
    }
return result;
}
//排序数字
void sort_numbers(std::vector<int> &input){
    //if(input.size>0){---------------------------错误
    if(input.size()>0){
    std::sort(input.begin(),input.end());
    }else{
        //printf("输入数据为空：无法排序");---------------------------------错误c++没有printf
        std::cout<<""<<std::endl;
    }
}

void print_statistics(const std::vector<int> &input){
  //printf("最小值：",std::cout<<input.front()<<'\n');//-------------错误c++没有printf-----------优化std::cout<<input.front()<<'\n'   '\n'
  std::cout<<"最小值："<<input.front()<<'\n';
  std::cout<<"最大值："<<input.back()<<'\n';
 //printf("最大值：",std::cout<<input.back()<<'\n');
 int sum=0,num=0;
 for(int num :input){
    sum=sum+num;
 }
double average=(double)sum/(double)input.size();//-------------------------------------错多次
//printf("平均值为："std::cout<<average<<'\n');
std::cout<<"平均值为："<<average<<'\n';
}



int main(void){
//从终端读取数字
int num;
std::vector<int> input;
//while(std::cin>>num)input.push_back(num);-----------------错误，为了省事
while(std::cin>>num){
    input.push_back(num);
};
//健壮性，输入为空怎么处理
if(input.size()==0){
    return 1;
}
//去重
std::vector<int> result=remove_duplicates(input);
//排序
sort_numbers(result);
//计算
print_statistics(result);
return 0;

}