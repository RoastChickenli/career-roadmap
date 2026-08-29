#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
//#include <unistd.h>getpid() getppid() fork() sleep()  exec...(); // 让当前进程运行另一个程序
//sys/types.h   → 放“Unix/Linux专用数据类型”的说明 pid_t 来自 <sys/types.h> getpid() 来自 <unistd.h>
#include<sys/wait.h>
//#include<sys/wait.h>     sys/wait.h = 父进程等待、回收和检查子进程的工具    wait() waitpid() WIFEXITED() WEXITSTATUS()
//函数----main在函数下面

int get_exit_code(void){

    return 7;
}



int main(void){
    //int exit_code=get_exit_code;-----------------------错误方法的使用
    int exit_code=get_exit_code();
    //printf("计划设置的子进程退出码为%d",exit_code/n);--------------错误换行符
    printf("计划设置的子进程退出码为%d\n",exit_code);


    pid_t current_pid=getpid();
    pid_t parent_pid=getppid();
    //printf("当前pid为:%d\n",current_pid);-----规范问题--强制转换
    printf("当前进程pid为:%d\n",(int)current_pid);
    printf("当前进程的父进程的pid为:%d\n",(int)parent_pid);

printf("\n");
//printf(\n);--------------------------错误
//printf(\n);
/*
你的这次执行时序
时间	父进程946390	子进程946391
①	打印“开始使用fork”	还不存在
②	调用 fork()	被创建
③	fork_result=946391	fork_result=0
④	调度器先运行父进程	等待CPU
⑤	进入 else 并打印	等待CPU
⑥	return 0，父进程结束	获得CPU
⑦	已结束	进入 fork_result==0 分支
⑧	已结束	打印并退出 
这一次Linux调度器恰好先选择了父进程。

*/
printf("\n");
printf("\n");
    printf("开始使用fork()\n");
    pid_t fork_result=fork();

    if(fork_result<0){
        perror("fork");
        return 1;
    }

    if(fork_result==0){//子进程执行的部分
        printf("[子进程] 当前PID为%d,fork返回值为%d\n",(int)getpid(),(int)fork_result);
        //子进程睡眠2s（模拟业务执行）
        sleep(2);
        return 7;
    }else{//父进程执行的部分
        printf("[父进程] 当前PID为%d,fork返回值为%d\n",(int)getpid(),(int)fork_result);
        //重点来了，从来没有见过。子进程与父进程怎么相互联系


        /*
        wait()的作用是：如果子进程还没有退出，父进程就在这里等待；子进程退出后，父进程将它回收，然后继续执行。
父进程调用wait(NULL)
        ↓
发现子进程还在休眠
        ↓
父进程进入等待状态
        ↓
子进程2秒后退出
        ↓
Linux唤醒父进程
        ↓
wait()返回子进程PID
        */
        //步骤一
        // pid_t waited_pid=wait(NULL);
        // //wait(NULL);这里的 NULL表示：只等待和回收子进程，暂时不保存子进程退出状态。
        // if(waited_pid <0){
        //     perror("wait");
        //     return 1;
        // }
        //   printf("[父进程] 已经回收子进程PID=%d\n",
        //        (int)waited_pid);

//重点---从来没学过--没见过        
// WIFEXITED  wait if exited 
//WEXITSTATUS  wait exit status
/*
&status：
这是一个指向整型变量的指针。
操作系统不会直接把“退出码”（比如 0 或 1）存进这个变量，---------------------------------这里没理解
而是存入一个包含丰富信息的状态值（比如子进程是被杀掉的、还是自己退出的、退出码是多少等）。
我们需要通过后面的宏（如 WIFEXITED）来“解码”这个状态值。
*/


     int status;
     pid_t waited_pid2 =wait(&status);
/*

wait()有没有成功？-------------------------wait()  返回值是结束的那个子进程的 PID   
        ↓ 成功
子进程是不是正常退出？(wait if exit)
        ↓ 是
正常退出码是多少？(wait exit status)
        ↓
得到7

WIFEXITED(status)
这是一个状态检查宏。
作用：判断子进程是否是正常退出的。
逻辑：如果子进程是调用 exit() 或从 main 函数返回而结束的，它返回真（非 0）；
如果是被信号（如 kill -9）强制杀死的，它返回假（0）。

WEXITSTATUS(status)
这是一个提取宏。
作用：只有当 WIFEXITED 为真时才能使用。它负责从复杂的 status 状态值中，
提取出子进程实际返回的那个数字（即 return 0 中的 0）。

*/




    if(waited_pid2 <0){
            perror("wait2");
            return 1;
        }
    printf("[父进程] 已回收子进程PID=%d\n",
               (int)waited_pid2);
    if(WIFEXITED(status)){
        int child_exit_code =WEXITSTATUS(status);
         printf("[父进程] 子进程正常退出，退出码为%d\n",
                   child_exit_code);
    }else{
        printf("[父进程] 子进程不是正常退出\n");
    }
/*

父进程调用fork()
        ↓
创建子进程960001
        ↓
子进程打印PID和PPID
        ↓
子进程sleep(2)
        ↓
父进程停在wait(&status)
        ↓
子进程执行return 7
        ↓
Linux把退出信息保存起来
        ↓
父进程的wait()返回
        ↓
WIFEXITED(status)判断为真
        ↓
WEXITSTATUS(status)提取出7
*/




    }


    return 0;
}
