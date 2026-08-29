#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_set>
#include<string>

//1.主函数输入数字--------------------------------------错误main（）函数要写到所有方法的最后面



//2.去重数字方法  错误的地方（unorde_set用法）
std::vector<int> renumber(std::vector<int> input){ 
    std::unordered_set<int> seen;
    std::vector<int> output;
    for(int num :input){
    //if(seen.find(num)==0){---------------错误
    if(seen.count(num)==0){
        seen.insert(num);
        output.push_back(num);
    }
   }
   for(int num :output)std::cout<<num; 
   return output;
}

//3.从小到大排序

//std::vector<int> sortnum(std::vector<int> input){//------------------------------------错误 &的用法
void sortnum(std::vector<int> &input){
    std::sort(input.begin(),input.end());
    for(int num :input)std::cout<<num;
}


//输出最大值最小值平均值
void calculate(std::vector<int> &input){//------------------------------------&的性能问题
    //std::cout<<"最小值："<<input.begin();
    //std::cout<<"最大值;"<<input.end();------------------------------------错误  vector()取值
    //最小值应该用 input.front()（排序后的第一个元素）
    //最大值应该用 input.back()（排序后的最后一个元素）

std::cout<<"最小值："<<input.front();
std::cout<<"最大值;"<<input.back();
    //std::int sum,add;------------------------------------错误 如何连续申请变量
    //std::int sum=0,add=0;------------------------------------错误int前面不需要加std::

    int sum=0,add=0;
    for(int num :input){
           sum=sum+num;
           add=add+1;
    }
    std::cout<<"平均值："<<sum/add;

}


//主函数
int main(){
    std::cout<<"请输入若干数字"<<std::endl;
    // int num;
    //num=std::>>cin;-------------------错误。这里与java存在不同
    int num;
    std::vector<int> input;
    //while(cin>>num){-------------------错误
    while(std::cin>>num){
        input.push_back(num);
    }
    std::cout<<"输入的数字:";
    for(int num : input){
        //std::cout>>num>>" "------错误
        std::cout<<num<<" ";
    }
 
    std::cout<<"数字去重";
    std::vector<int> output=renumber(input);

    std::cout<<"排序从小到大";
    sortnum(output);
    calculate(output);
    return 0;
}
