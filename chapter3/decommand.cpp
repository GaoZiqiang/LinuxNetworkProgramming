// decommand.cpp--仿真Linux调用系统

#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

int main() {
    int pid;
    char command[] = "pwd";

    pid = fork();
    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1);

        case 0:
            // 没有通过exec直接执行command命令，而是通过exec去执行shell(即/bin/sh)，再使用shell执行指定的命令command
            execl("/bin/sh","sh","-c",command, nullptr);
            perror("execl failed");
            exit(127);

        default:
            // 父进程为什么要sleep或wai
            // 答：因为父进程是主进程，若父进程/主进程运行结束--其相关资源会被释放掉？，则子进程也被迫结束
//            sleep(1);
            wait(nullptr);// wait()返回第一个结束的子进程的标识符
//            printf("ls comspleted\n");
//            exit(0);
    }

    return 0;
}