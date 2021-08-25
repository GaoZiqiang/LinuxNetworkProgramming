#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char * argv[]) {
    char message[] = "hello client!";
    int sockfd, client_fd;

    struct sockaddr_in my_addr;// 本地地址信息
    struct sockaddr_in client_addr;// 客户端连接的地址信息
    int addr_length;// 网络地址长度

//    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");// 将上一个函数发生错误的原因输出到标准设备(stderr)
        exit(EXIT_FAILURE);
    } else {
        printf("socket success\n");
    }

    memset(&my_addr, 0, sizeof(my_addr));// 初始化为0
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;// 自动IP地址获得
    my_addr.sin_port = htons(atoi(argv[1]));// 获取端口号--第二个参数

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    socklen_t client_addr_length = sizeof(client_addr);
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_length);
    if (client_fd == -1) {
        perror("accept error");
        exit(EXIT_FAILURE);
    }
    printf("client_fd: %d\n",client_fd);

    write(client_fd, message, sizeof(message));

    close(client_fd);
    close(sockfd);
}