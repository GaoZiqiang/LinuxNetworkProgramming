#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "list_timer.h"
#include "keep_alive.h"

#define FD_LIMIT 65535// 最大连接数
#define MAX_EVENT_NUMBER 1024

static timer_signal timer_signal;
static timer_list timer_list;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s ip port\n", basename(argv[0]));
        return 1;
    }
    // 生成pipefd
    timer_signal::get_pipefd();

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int servfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(servfd >= 0);

    ret = bind(servfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(servfd, 5);
    assert(ret != -1);

    // 存放相应事件--传出参数
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    // 将servfd添加到epoll空间
    timer_signal.add_fd(epollfd, servfd);

    timer_signal.set_non_blocking(pipefd[1]);// pipe写端设置非阻塞
    // 将pipe读端加入epoll空间
    timer_signal.add_fd(epollfd, pipefd[0]);

    // 信号注册
    timer_signal.add_sig(SIGALRM);
    timer_signal.add_sig(SIGTERM);
    bool stop_server = false;

    client_data* users = new client_data[FD_LIMIT];
    // 超时标志信号
    bool timeout = false;
    // 启动定时器
    alarm(TIMESLOT);

    while (!stop_server)
    {
        int ready_num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);// 无限等待
        if ((ready_num < 0) && (errno != EINTR))
        {
            printf("epoll failuer\n");
            break;
        }

        for (int i = 0; i < ready_num; i++)
        {
            int sockfd = events[i].data.fd;
            printf("sockfd: %d\n", sockfd);
            // 新连接客户端
            if (sockfd == servfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int clientfd = accept(servfd, (struct sockaddr*)&client_address, &client_addrlength);
                printf("新客户端 %d 连接成功\n", clientfd);
                // 将clientfd添加到epoll空间
                timer_signal.add_fd(epollfd, clientfd);
                // 填充client_data
                users[clientfd].address = client_address;
                users[clientfd].sockfd =clientfd;

                // 填充定时器timer_node
                timer_node* timer = new timer_node;
                timer->user_data = &users[clientfd];
                timer->cb_func = timer_signal::cb_func;
                time_t cur_time = time(nullptr);// 获取系统当前时间
                timer->expire = cur_time + 3 * TIMESLOT;// 设置定时器到期时间

                users[clientfd].timer = timer;

                // 添加到定时器链表
                timer_list.add_timer(timer);
            }
            // 处理SIGALRM信号
            else if ((sockfd == pipefd[0]) && (events[i].events && EPOLLIN))
            {
                printf("接收到SIGALRM信号\n");
                int sig;
                char signals[1024];// 收到的信号放到数组中--一个信号一个字节char
                ret = recv(pipefd[0], signals, sizeof(signals), 0);
                if (ret == -1)
                {
                    continue;
                }
                else if (ret == 0)
                {
                    continue;
                }
                else
                {
                    for (int i = 0; i < ret; i++)
                    {
                        switch(signals[i])
                        {
                            case SIGALRM:
                               timeout = true;
                               break;

                            case SIGTERM:
                                stop_server = true;// 强制关闭服务器主进程
                        }
                    }

                }

            }
            // 处理客户端发来的数据
            else if (events[i].events & EPOLLIN)
            {
                int clientfd = events[i].data.fd;
                printf("接收到客户端 %d 数据\n", clientfd);
                memset(users[clientfd].buf, 0, BUF_SIZE);// sockfd即clientfd
                ret = recv(clientfd, users[clientfd].buf, BUF_SIZE - 1, 0);// 接收的数据保存到users[sockfd].buf
//                printf("get %d bytes data from client %d: %s\n", ret, sockfd, users[sockfd].buf);

                timer_node* timer = users[clientfd].timer;
                // 发生读错误
                if (ret < 0)
                {
                    if (errno != EAGAIN)
                    {
                        // 关闭该client sockfd
                        timer_signal.cb_func(&users[clientfd], epollfd);
                        // 从timer_list链表中删除timer
                        if (timer)
                            timer_list.del_timer(timer);
                    }
                }
                // 对方已关闭连接
                else if (ret == 0)
                {
                    timer_signal.cb_func(&users[clientfd], epollfd);
                    if (timer)
                        timer_list.del_timer(timer);
                }
                // 有数据可读
                else
                {
                    printf("get %d bytes data from client %d: %s\n", ret, clientfd, users[clientfd].buf);
                    // 调整定时器
                    if (timer)
                    {
                        time_t cur_time = time(nullptr);
                        timer->expire = cur_time + 3 * TIMESLOT;// 再续3 * TIMESLOT
                        printf("adjust timer once\n");
                        timer_list.adjust_timer(timer);
                    }
                }
            }
            else
            {
                // others
            }

        }
        // 待所有ready_num都处理完再处理超时定时器
        if (timeout)
        {
            timer_signal.timer_handler(timer_list, epollfd);
            timeout = false;
        }
    }

    close(servfd);
    close(pipefd[0]);
    close(pipefd[1]);
    delete[] users;

    return 0;
}
