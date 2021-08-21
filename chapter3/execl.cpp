#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Executing ls\n");
    execl("/bin/pwd","pwd", nullptr);

    // 如果execl返回，说明调用失败
    perror("execl failded to run ls\n");

    return 0;
}
