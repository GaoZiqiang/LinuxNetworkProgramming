#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/wait.h>

// 错误处理
void failure(char * s) {
    perror(s);
    exit(1);
}

void printpos(char * str,int fildes) {
    long pos;
    if ((pos = lseek(fildes, 0L, 1)) < 0L)
        failure("lseek failed");
    printf("%s: %ld\n",str,pos);
}

int main() {
    int fd;// 文件描述符
    int pid;
    char buf[10];// 数据缓冲区

    if ((fd = open("data.txt",O_RDONLY)) < 0)
        failure("open failed");
    else
        printpos("Before read",fd);

    read(fd, buf, 10);
    printpos("Before fork",fd);

    // fork新进程
    if ((pid = fork()) < 0)
        failure("fork failed");
    else if (!pid) {// 子进程
        printpos("Child before read",fd);
        read(fd, buf, 10);
        printpos("Child after read",fd);
    } else {// 父进程
        wait(nullptr);
        printpos("Parent after wait",fd);
    }
}
