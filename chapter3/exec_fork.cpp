#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

int main() {
    int pid;

    pid = fork();
    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1);

        case 0:
            execl("/bin/ls","ls","-l", nullptr);
            perror("execl failed");
            exit(1);

        default:
            printf("child pid: %d\n",pid);
            // 父进程为什么要sleep或wai
            // 答：因为父进程是主进程，若父进程/主进程运行结束--其相关资源会被释放掉？，则子进程也被迫结束
//            sleep(1);
            int a = 1;
            // wait()的返回值通常是结束的那个子进程的进程标识符
            printf("wait result: %d\n",wait(&a));// 子进程在执行完，运行exit()会重新启动父进程运行
            printf("ls comspleted\n");
            exit(0);
    }

    return 0;
}