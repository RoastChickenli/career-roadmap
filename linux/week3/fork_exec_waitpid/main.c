/*
第三题：fork + exec + waitpid

编写一个C程序：

父进程调用fork()创建一个子进程；
子进程输出自己的PID和父进程PID；
子进程调用execlp()执行另一个程序；

新程序输出一句话；

父进程使用waitpid()等待指定子进程；
父进程判断子进程如何结束，并输出退出码。

1.fork()返回值在父进程和子进程中分别是什么？

父进程：子进程pid
子进程：0
2.exec()会不会创建一个新进程？
不会
3.exec()成功后，子进程PID会不会改变？
不会
4.为什么execlp()成功后，后面的代码不会执行？

因为 exec 的本质是“替换”。它会把当前进程的内存（代码段、数据段、堆栈）全部擦除，换成新程序的内容。
5.execlp()名称末尾的p表示什么？

p 代表 PATH。
带 p：系统会在环境变量 PATH 指定的目录中搜索该程序（比如 ls, echo 这些命令）。
不带 p（如 execl）：你必须提供文件的完整路径或相对路径（比如 ./child_prog 或 /bin/ls）
6.execlp()参数中的第二个"echo"有什么作用？

按照 Unix 惯例，argv[0] 通常是程序名本身。
7.为什么execlp()失败后使用_exit(127)？

为什么是 127？ 在 Shell 中，如果找不到命令，默认返回的退出码就是 127。这是一种约定俗成的标准，方便父进程判断是“命令不存在”还是“程序运行出错”。
为什么用 _exit() 而不是 exit()？ 因为子进程已经通过 fork 复制了父进程的文件描述符和缓冲区。
如果调用 exit()，可能会刷新父进程的 I/O 缓冲区，导致输出重复或混乱。_exit() 会直接终止进程，不进行缓冲区清理，更加安全。
8.waitpid(child_pid, &status, 0)三个参数分别是什么？

child_pid：等待的子进程pid号
&status：返回的运行状态指针
0：阻塞等待
9.为什么这里使用waitpid()而不是wait()？

wait()：无序的。只能等待任意一个子进程。如果你创建了多个子进程，你无法控制先回收哪一个。
waitpid()：有序的。可以指定等待特定的子进程，在需要精确控制进程回收顺序时，必须用 waitpid
10.WIFEXITED、WEXITSTATUS、WIFSIGNALED和WTERMSIG分别做什么？

WIFEXITED(status)：判断子进程是否正常退出（调用了 exit() 或 return）
WEXITSTATUS(status)：获取子进程的退出码（0-255）
WIFSIGNALED(status)：判断子进程是否被信号杀死（如 Ctrl+C 产生的 SIGINT）
WTERMSIG(status)：获取导致子进程终止的信号编号
*/



//子进程调用execlp()执行另一个程序；
#include<stdio.h>
#include <unistd.h>   // fork, execlp, getpid
#include <sys/wait.h> // waitpid, WIFEXITED
#include <stdlib.h>   // exit
int main(){
    //fork()出进程
    pid_t child_pid=fork();
//--------------------------------------------处理fork()失败
if(child_pid<0){
perror("fork filed");

return 1//----------------------------出错后直接结束
}



    if(child_pid==0){
        //子进程
        // 子进程输出自己的PID和父进程PID；
        printf("子进程PID:%d,父进程PID:%d\n",(int)getpid(),(int)getppid());
        //子进程调用execlp()执行另一个程序；----------------------------------------------这里不会execlp()用法不会
/*
 execlp(
    "要执行的程序",
    "传给新程序的argv[0]",//一般是程序的名称
    "参数1",
    "参数2",
    (char *)NULL
    );
 */
 fflush(stdout);//----------------------------stdout就是“标准输出”，一般指终端屏幕。printf()--->先放进输出缓冲区--->再统一显示到终端
execlp("./child_program","child_program",(char *)NULL);
  perror("execlp");
 _exit(127);//----------------------------------------------在 execlp() 执行失败后，立即结束子进程，并把退出码设为 127       
    }else{
        int status;
        pid_t wpid;
        //父进程
        //父进程使用waitpid()等待指定子进程；
        wpid=waitpid(child_pid,&status,0);
        if(wpid<0){
            //perror(waitpid);-------------------------------------错误perror参数必须是字符串
            perror("waitpid failed\n");
            return 1;//------------------------------------出错后直接结束程序
        }
        if(WIFEXITED(status)>0){//-----------------错误：宏定义只需呀值
            printf("子进程成功顺利退出PID:%d，退出码为：%d\n",(int)wpid,(int)WEXITSTATUS(status));

        }else{
             printf("子进程不正常退出");
        }

        //父进程判断子进程如何结束，并输出退出码。
    }

     return 0;
}