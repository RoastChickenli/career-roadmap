/*
1.pipe_fd[0]和pipe_fd[1]分别是什么？

pipe_fd[0]管道的读端
pipe_fd[1]管道的写端
2.为什么必须在fork()之前调用pipe_fd[]？

继承”。
pipe_fd[] 只是一个普通的局部数组，它本身并不具备跨进程通信的能力。
关键在于 fork() 会复制父进程的文件描述符表。只有在 fork() 之前调用 pipe()，子进程才能通过复制，拿到指向同一个内核管道对象的文件描述符。
如果先 fork() 再 pipe()，父子进程各自创建的管道是完全独立的，无法通信。
3.fork()以后，父子进程分别拥有哪些管道文件描述符？

父进程：pipe_fd[0]pipe_fd[1]
子进程：pipe_fd[0]pipe_fd[1]
4.为什么父进程需要关闭pipe_fd[0]？
父进程只写，需要关闭读

5.为什么子进程需要关闭pipe_fd[1]？
子进程读需要关闭写

6.如果所有写端都已关闭，read()会返回什么？
0
7.read()返回-1、0和正数分别代表什么？
-1：读取出错。
0：所有写端都已关闭（EOF）
正数：读到的字节数
8.为什么读取时使用sizeof(buffer) - 1？

要空出最后一位，放结束符'\0'
9.为什么需要执行buffer[bytes_read] = '\0'？

将最后一位存为结束符
10.匿名管道默认是单向还是双向通信？
单向
11.如果需要父进程和子进程互相发送消息，需要几个管道？
两个
12.为什么管道不能直接使用普通的全局数组代替？
fork() 之后，父子进程拥有各自独立的内存空间。全局数组在子进程中只是父进程数组的一份副本，子进程修改它，父进程根本看不到。
而管道是操作系统内核提供的一块共享缓冲区，才能真正实现进程间的数据传递。

编写一个C程序：
调用pipe()创建匿名管道；
调用fork()创建子进程；
父进程向管道写入一条任务信息；
子进程从管道读取任务信息；
父子进程分别关闭自己不使用的管道端；
父进程使用waitpid()回收子进程；
输出子进程退出状态。

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(){
  

    //1.调用pipe()创建匿名管道；---------------------------------创建管道不会

    //1. 声明一个数组来存放管道的两个文件描述符
    int pipe_fd[2];  // // pipe_fd[0] 是读端，pipe_fd[1] 是写端
    
    
    
    // 2. 创建管道
    if(pipe(pipe_fd)<0){
        perror("pipe创建失败");
        return 1;
    }



    //调用fork()创建子进程；  
    pid_t child_pid;
    child_pid=fork();
    if(child_pid<0){
        //创建的管道需要关闭
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        perror("fork filed");
        return 1;
    }
    if(child_pid==0){
        //子进程------------------------读取管道内容
        //子进程不写，所以关闭管道写段
        close(pipe_fd[1]);

        char buffer[128];
        //从读端读取数据-----------------------------read()方法使用  
 /*
        pipe_fd[0]：从哪里读？ 从管道的读端读。
        buffer：读到哪去？ 读到我们提前准备好的字符数组（缓冲区）里。
        sizeof(buffer) - 1：最多读多少？ 最多读 buffer 的大小减 1 个字节。
为什么要 - 1？
这是为了给字符串的结束符 \0 留位置。
假设你的 buffer 大小是 128 字节。如果你一口气读满了 128 个字节，那就没有空间放 \0 了。
后面如果你用 printf("%s", buffer) 输出，程序就会因为找不到结束符而乱码甚至崩溃。


ssize_t：这是一个专门用来表示“字节数”的数据类型（signed size_t）。
它可以是正数，也可以是负数。
 */

        ssize_t bytes_read=read(pipe_fd[0],buffer,sizeof(buffer)-1);
        if(bytes_read>0){
            buffer[bytes_read]='\0';//-------------------在读出内容的最后一位加上结束符号'\0'
            printf("子进程收到消息%s\n",buffer);
        }


        //读完  关闭读端
        close(pipe_fd[0]);
        _exit(0); // 子进程退出

    }else{


        printf("[父进程] 已创建子进程PID=%d",(int)child_pid);
    //父进程
        //父进程不读，关闭读端
        close(pipe_fd[0]);
        const char *msg="hello from parent!";
        //向管道写数据-----------------------------------write()方法
        /*
ipe_fd[1]：往哪里写？ 往管道的写端写。
msg：写什么？ 写 msg 这个指针指向的内存里的数据（也就是你的字符串）。
strlen(msg)：写多少？ 写 msg 字符串的实际长度（不包含结尾的 \0）。
2. 为什么要用 strlen(msg)？
因为管道传输的是纯粹的字节流，它不认得什么是“字符串”，只认得“字节”。
strlen(msg) 会计算出字符串里到底有多少个有效字符。比如 "Hello" 的长度是 5，write 就会精确地把这 5 个字节塞进管道里。
        
        
        */
        
      ssize_t written =  write(pipe_fd[1],msg,strlen(msg));

pid_t wpid;
        int status;
        //write错误判断
        if (written < 0) {
    perror("write failed");
    close(pipe_fd[1]);
    waitpid(child_pid, &status, 0); // 别忘了回收子进程
    return 1;
}



        printf("[父进程] 消息已发送\n");

          // 写完了，关闭写端
        close(pipe_fd[1]);
        
        wpid=waitpid(child_pid,&status,0);
        if(wpid<0){
            perror("waitpid filed");
            return 1;
        }
        if(WIFEXITED(status)>0){
          printf("子进程正常退出，退出码：%d",WEXITSTATUS(status));
        }else{

            printf("子进程退出异常");
            
        }


    }

    return 0;
}