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
#include <stdio.h>
#include <pthread.h>

#include "keep_alive.h"

int pipefd[2];

timer_signal::timer_signal()
{

}

timer_signal::~timer_signal()
{

}

void timer_signal::get_pipefd()
{
    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret != -1);
}

int timer_signal::set_non_blocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 将事件添加到epoll空间
void timer_signal::add_fd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;// 边缘触发
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_non_blocking(fd);
}

void timer_signal::sig_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], (char*)&msg, 1, 0);
    errno = save_errno;
}

void timer_signal::add_sig(int sig)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    // 设置信号处理函数
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;

    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, nullptr) != -1);
}

void timer_signal::timer_handler(timer_list &timer_list, int epollfd)
{
    // 每次收到SIGALRM信号便触发一次tick()脉搏函数
    timer_list.tick(epollfd);
    // 重新设置一个闹钟
    alarm(TIMESLOT);
}

// 实体回调函数--timer_node中的cb_func只是一个“形式”？
void timer_signal::cb_func(client_data* user_data, int epollfd)
{
    // 将闹钟绑定的客户端的sockfd从epoll空间中移除
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    printf("close client fd %d\n", user_data->sockfd);
}
