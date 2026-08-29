#include<iostream> //1. 第一行：包含标准输入输出头文件
#include<string> // 2. 如果要读取字符串，引入 string 头文件
int main(){
    std::cout<<"请输入一个整数";
    int num=0;
    std::cin>> num;
    std::cout<<"输入的数字是"<<num<<std::endl;
    return 0;
}