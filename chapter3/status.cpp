#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main() {
    int pid, status, exit_status;

    if (pid = fork() < 0) {
        perror("fork failed");
        exit(1);
    }

    if (!pid) {
        sleep(4);
        exit(5);
    }

    if (wait(&status) < 0) {
        perror("wait failed");
        exit(1);
    }
    printf("status: %d\n",status);
}