#include<iostream>
#include<vector>//使用vector必须引入头文件    解决int number[10]  string arr[10]等无固定长度无法扩容
//把 vector 想象成一个弹性的抽屉盒
#include<string>
//#include<sort>--------------------------错误
#include<algorithm>
#include<unordered_set>
/*

操作类型	函数 / 语法	说明
尾部添加	v.push_back(val)	在末尾插入一个元素（最常用）
尾部删除	v.pop_back()	删除末尾最后一个元素
指定位置插入	v.insert(v.begin() + i, val)	在下标为 i 的位置插入元素
指定位置删除	v.erase(v.begin() + i)	删除下标为 i 的元素
区间删除	v.erase(v.begin() + i, v.begin() + j)	删除下标从 i 到 j-1 的所有元素
清空所有	v.clear()	删除所有元素，清空容器
访问/修改	v[i] 或 v.at(i)	访问或修改下标为 i 的元素
首尾访问	v.front() / v.back()	获取第一个元素 / 最后一个元素
获取大小	v.size()	获取当前元素个数
检查是否为空	v.empty()	如果为空返回 true，否则返回 false
*/



int main(){
    std::cout<<"输入若干整数使用vector";
    std::vector<int> nums;
    for(int i=1;i<=5;i++){
        nums.push_back(i);
    }
    //std::cout<<nums;------错误    
    std::cout<<"vector内容";
    for(int num: nums){
        std::cout<<num<<" ";
    }    
    std::cout<<std::endl;
   

    std::cout<<"question2";
    std::vector<int> nums2;
    nums2={10,20,30,40,50};
    //nums2.pop;---------------------错误
    nums2.pop_back();
    for(int num :nums2){
        //std::cout<<num;---------------------错误
        std::cout<<num<<" ";
    }
    //std::cout<<endl;---------------------错误
    std::cout<<std::endl;
    //nums2.pop(0);---------------------错误
    nums2.erase(nums2.begin());
    for(int num :nums2){
        std::cout<<num<<" ";
    } 
    std::cout<<std::endl;

std::cout<<"question3";

    //vector<vector<int>> matrix;--错误
    //vector<int> num1={1,2};--错误
   // vector<int> num2={3,4,5};--错误
   //std::vector<vector<int>> matrix;------错误
   std::vector<std::vector<int>> matrix;
   std::vector<int> num1={1,2};
   std::vector<int> num2={3,4,5};
    matrix.push_back(num1);
    matrix.push_back(num2);

    //for(vector<int> nums :matrix){--错误
    for(std::vector<int> nums :matrix){    
        for(int num :nums){
            std::cout<<num<<" ";
        }
        std::cout<<std::endl;
    }
 
    




std::cout<<"question4";
std::cout<<std::endl;
std::vector<int> list1={1,7,1,9,2,2,2,3};

//std::sort(list);---------------------错误sort
std::sort(list1.begin(),list1.end());
//auto last=std::unique(list1);---------------------错误
auto last=std::unique(list1.begin(),list1.end());


//std::vector<int> result=erase(list1.last,list1.end);---------------------错误
//std::vector<int> result=list1.erase(last,list1.end());---------------------错误
list1.erase(last,list1.end());
for(int num :list1){
    std::cout<<num;
}
std::cout<<std::endl;

std::cout<<"question4.5";
//数字去重后保持原顺序
std::vector<int> list2={1,7,1,9,2,2,2,3};
//std::unordered_set<int> seen;---------------------错误
std::unordered_set<int> seen;
std::vector<int> result;
for(int num:list2){
    if(seen.count(num)==0){//---------------------  unordered_set  insert count find
        seen.insert(num);
        result.push_back(num);
    }
}
for(int num :result)std::cout<<num;
std::cout<<std::endl;

    return 0;
}