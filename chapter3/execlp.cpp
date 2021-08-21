#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Executing pwd\n");
    execlp("pwd","pwd", nullptr);

    // 如果execl返回，说明调用失败
    perror("execl failded to run pwd\n");
    // 这句话就没有执行的机会了
    printf("end execlp\n");

    return 0;
}