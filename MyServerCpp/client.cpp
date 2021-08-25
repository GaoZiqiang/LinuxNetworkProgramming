#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 2

int main(int argc, char * argv[]) {
    char message[BUFSIZ];
    int ret = 0;
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("sockfd error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    printf("connect success\n");

    int read_len;
    while ((read_len = read(sockfd, message, BUFSIZE)) != 0) {
        if (read_len == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }// 注意：read函数返回值str_len==0和==-1不一样，返回-1表示读取出现问题，而返回0表示文件读取长度为0--读取完了

        printf("message from server: %s\n",message);
    }
    printf("final str_len: %d\n",read_len);

    close(sockfd);
}
