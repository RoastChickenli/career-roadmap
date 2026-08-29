#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h> // 必须包含，否则 fork/sleep/getpid 未声明
/*
程序要求:
父进程连续创建3个子进程：

子进程编号	休眠时间	退出码
1	1秒	11
2	2秒	22
3	3秒	33

每个子进程需要输出：

[子进程1] PID=xxx，父进程PID=xxx，休眠1秒

休眠结束后输出：

[子进程1] 执行结束，退出码=11

父进程创建完3个子进程后，必须：

使用wait()回收全部子进程；
输出被回收子进程的PID；
使用WIFEXITED(status)判断是否正常退出；
使用WEXITSTATUS(status)取得退出码；
最后输出：
[父进程] 所有子进程均已回收

1.为什么父进程需要调用3次wait()？

因为fork()三次,产生3个子进程需要3次wait()回收
2.为什么子进程分支执行结束后必须立即return？----------------------------------大错
（如果不return for()只能创建一个进程,后面的for无法继续进行）

如果不return结束，子进程会继续向下执行for循环


3.wait()返回的数值是什么？
子进程的pid
4.status变量是不是直接等于子进程退出码？
status是int变量；
&status才是指向它的指针；
wait()通过这个地址把子进程状态写进status。

&status是一个指向整型变量的指针，包含丰富信息的状态值（比如子进程是被杀掉的、还是自己退出的、退出码是多少等）。

5.WIFEXITED(status)和WEXITSTATUS(status)分别负责什么？
WIFEXITED(status)复制判断子进程是否正常退出
WEXITSTATUS(status)提取子进程退出码
6.子进程的创建顺序、执行顺序和退出顺序一定相同吗？为什么？
执行顺序 不一定 子进程是并行产生，执行顺序与cpu的调度有关
创建顺序 一定
退出顺序 一定（睡眠的时间递增）


*/

int main(void){

    for(int i=0;i<3;i++){

        pid_t pid=fork();
        if(pid<0){
            //printf("fork失败,返回码1");-----------------------------------------perror("fork");printf不明显
            perror("fork");
            return 1;
        }
        if(pid==0){
           printf("[子进程%d] PID=%d，父进程PID=%d，休眠%d秒\n",i+1,(int)getpid(),(int)getppid(),i+1);//------错误  getpid()返回pid_t 类型需要转换成int
           sleep(i+1);
            printf("[子进程%d] 执行结束，退出码=%d\n",i+1,(i+1)*10+(i+1));
            return (i+1)*10+(i+1);//-----------------------------------------错误，没有退出子进程必须要有出口
        }
    }
    int status;
    pid_t child_pid;
    // wait() 每次只回收一个，必须循环直到没有子进程可回收
    while((child_pid=wait(&status))>0){
        printf("[父进程] 回收子进程 PID=%d\n",child_pid);
        if(WIFEXITED(status)){
            printf("[父进程] 子进程正常退出，退出码=%d\n",WEXITSTATUS(status));
        }else{
            printf("[父进程] 子进程异常终止\n");
        }
    }
    printf("[父进程] 所有子进程均已回收\n");
    return 0;

}