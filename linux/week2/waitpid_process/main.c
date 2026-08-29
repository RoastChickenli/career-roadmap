#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h> 
/*
wait()和waitpid()最主要的区别是什么？
wait()：无法指定接收顺序。它会阻塞父进程，直到任意一个子进程结束，并返回该子进程的 PID。
waitpid()：是“精准回收”。它允许父进程指定等待特定 PID 的子进程
waitpid(pid, &status, 0)中的三个参数分别表示什么？
pid：等待接收的pid
status：保存子进程的运行状态
0：阻塞和非阻塞 0阻塞
第三个参数0表示什么？
父进程阻塞等待
为什么child_pids[i] = pid只能放在父进程执行路径中？
child_pids[i] 是全局变量。子进程fork（）运行结束后退出，无法同步到全局变量中
子进程1先退出，但父进程正在等待子进程3时，子进程1会发生什么？
子进程1处于僵尸态度
waitpid()成功时返回什么？失败时返回什么？
成功：等待的pid 
失败：-1

一、题目要求
仍然创建3个子进程：

子进程	休眠时间	退出码
1	1秒	11
2	2秒	22
3	3秒	33

父进程需要保存每个子进程的PID：

pid_t child_pids[3];

每次fork()成功后，在父进程中保存：

child_pids[i] = pid;

创建完成后，父进程故意按照相反顺序回收：

子进程3 → 子进程2 → 子进程1

必须使用：

waitpid(child_pids[i], &status, 0);

不能使用wait()。

wait() 与 waitpid() 的本质区别
wait()：是“盲等”。它会阻塞父进程，直到任意一个子进程结束，并返回该子进程的 PID。你无法控制回收的顺序。
waitpid(pid, ...)：是“精准回收”。它允许父进程指定等待特定 PID 的子进程。
*/
int main(void){
    //pid_t child_pid[3];----------------------c语言如何申请申请数组
    pid_t child_pid[3];
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


            //child_pid[i]=getpid();-----------大错误，理解错误
            return (i+1)*10+(i+1);//-----------------------------------------错误，没有退出子进程必须要有出口
        }
        child_pid[i]=pid;
    }

    //使用for循环进行回收
    // wait() 每次只回收一个，必须循环直到没有子进程可回收
    // for (int i = 0; i < 3; i++) {
    // int status;
    // pid_t child_pid = wait(&status);

    // if (child_pid < 0) {
    //     perror("wait");
    //     return 1;
    // }

    // printf(
    //     "[父进程] 回收子进程PID=%d\n",
    //     (int)child_pid
    // );

    // if (WIFEXITED(status)) {
    //     printf(
    //         "[父进程] 子进程正常退出，退出码=%d\n",
    //         WEXITSTATUS(status)
    //     );
    // } else {
    //     printf("[父进程] 子进程异常终止\n");
    //  }
    // }

  
    /*
    pid_t waitpid(pid_t pid, int *status, int options);   成功时返回被回收子进程的 PID，失败时返回 -1。
下面为你详细拆解这三个参数在系统底层的含义：
1. pid_t pid（指定回收哪个进程）
这个参数决定了你要“收尸”的目标：
> 0：精准回收。等待进程 ID 等于这个值的子进程（你题目中要求的用法）。
-1：回收任意一个。效果等同于 wait()，只要有任何一个子进程退出就返回。
0：回收同一个进程组里的任意子进程。
< -1：回收指定进程组里的任意子进程。
2. int *status（获取子进程的退出状态）
这是一个指针，用来接收子进程结束时的状态信息。内核会把状态码写进这块内存，通常配合以下宏来解析：
WIFEXITED(status)：判断子进程是不是正常退出的。
WEXITSTATUS(status)：如果正常退出，提取它的退出码（比如你题目里的 11, 22, 33）。
WIFSIGNALED(status)：判断子进程是不是被信号异常杀死的。
3. int options（控制等待的行为）
这个参数决定了父进程在等待时的底层行为：
0：阻塞等待。父进程会挂起，直到指定的子进程结束（你题目中要求的用法）。
WNOHANG：非阻塞等待。如果指定的子进程还没结束，waitpid() 会立即返回 0，不会让父进程干等。这在父进程需要同时处理其他任务时非常有用。
    */
    //使用waitpid()回收
    for(int i=2;i>=0;i--){
        printf("[父进程] 准备等待子进程%d",i+1);
        
        int status;
        pid_t childpid;
        childpid=waitpid(child_pid[i],&status,0);
        
    if(childpid<0){
        perror("waitpid");
    }else{//-------------------------if()...else if()  else if()想要使用必须添加条件
        if(WIFEXITED(status)){//------------------------------------------这里记，出问题。
        printf("已回收子进程%d，PID=%d，退出码=%d\n",i+1,child_pid[i],WEXITSTATUS(status));
        }else{
             printf("子进程退出异常");
        }
    }

    }
    printf("[父进程] 所有子进程均已回收\n");
    return 0;

}