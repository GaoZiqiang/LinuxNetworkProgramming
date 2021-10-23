#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  //for htonl() and htons()
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>     //for signal()
#include <pthread.h>
#include <semaphore.h>
#include <list>
#include <errno.h>
#include <time.h>
#include <sstream>
#include <iomanip> //for std::setw()/setfill()
#include <stdlib.h>

#define WORKER_THREAD_NUM 5
#define EPOLL_SIZE 50
#define BUF_SIZE 256

int listen_fd = 0;
int epoll_fd = 0;
bool stop = false;

pthread_t accept_id = 0;
pthread_t worker_ids[WORKER_THREAD_NUM] = {0};

pthread_mutex_t accept_mutex;
pthread_mutex_t client_mutex;

pthread_cond_t accept_cond;
pthread_cond_t client_cond;

std::list<int> clients_list;

void destroy(int signo) {
    ::signal(SIGINT, SIG_IGN);
    ::signal(SIGKILL, SIG_IGN);
    ::signal(SIGTERM, SIG_IGN);

    printf("program recv signal: to exit%d\n", signo);

    stop = true;

    ::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, listen_fd, NULL);

    ::shutdown(listen_fd, SHUT_RDWR);
    ::close(listen_fd);
    close(epoll_fd);

    ::pthread_cond_destroy(&accept_cond);
    ::pthread_mutex_destroy(&accept_mutex);

    ::pthread_cond_destroy(&client_cond);
    ::pthread_mutex_destroy(&client_mutex);
};
bool createServerListener(const char* ip, int port) {
    listen_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (listen_fd == -1) {
        printf("socket fail\n");
        return false;
    }

    // 设置socket属性
    int on = 1;
    ::setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    ::setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, (char*)&on, sizeof(on));

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    if (::bind(listen_fd, (sockaddr*)&servaddr, sizeof(servaddr))) {
        printf("bind fail\n");
        return false;
    }

    if (::listen(listen_fd, 10) == -1) {
        printf("listen fail\n");
        return false;
    }

    epoll_fd = ::epoll_create(EPOLL_SIZE);
    if (epoll_fd == -1) {
        printf("epoll_create fail\n");
        return false;
    }

    // 待注册事件
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(ep_event));
    ep_event.events = EPOLLIN | EPOLLRDHUP;
    ep_event.data.fd = listen_fd;
    // 注册事件到例程
    if (::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ep_event) == -1) {
        printf("epoll_ctl fail\n");
        return false;
    }

    return true;
};

void releaseClient(int client_fd) {
    // 先从例程空间中销毁client_fd
    if (::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1)
        printf("client_fd epoll_ctl del fail\n");

    ::close(client_fd);
};

// 回调函数
void* acceptThreadFunc(void* arg) {
    while (!stop) {
        ::pthread_mutex_lock(&accept_mutex);
        // 阻塞等待accept_cond
        ::pthread_cond_wait(&accept_cond, &accept_mutex);

        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int new_client_fd = ::accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);

        ::pthread_mutex_unlock(&accept_mutex);

        if (new_client_fd == -1) {
            printf("accept fail\n");
            continue;
        }
        printf("new client connected: %s : %d\n", ::inet_ntoa(client_addr.sin_addr),::ntohs(client_addr.sin_port));


        // 将新socket设置为non-blocking
        int old_flag = ::fcntl(new_client_fd, F_GETFL, 0);
        int new_flag = old_flag | O_NONBLOCK;
        if (::fcntl(new_client_fd, F_SETFL, new_flag) == -1) {
            printf("fcntl fail, old_flag = %d, new_flag = %d", old_flag,new_flag);
            continue;
        }

        struct epoll_event ep_event;
        memset(&ep_event, 0, sizeof(ep_event));
        ep_event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        ep_event.data.fd = new_client_fd;
        // 注册到例程空间中
        if (::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client_fd, &ep_event) == -1) {
            printf("new_client_fd %d epoll_ctl fail\n", new_client_fd);
            continue;
        }
    }

    return NULL;
};

void* workerThreadFunc(void* arg) {
    // 线程在完成一项任务之后继续wait其他任务--充分利用一个线程
    while (!stop) {
        // 保护clients_list队列--一次只能一个线程访问clients_list队列
        ::pthread_mutex_lock(&client_mutex);
        while (clients_list.empty())
            ::pthread_cond_wait(&client_cond, &client_mutex);
        int client_fd = clients_list.front();// 取出一个client_fd
        clients_list.pop_front();
        ::pthread_mutex_unlock(&client_mutex);

        std::string client_msg;
        char buf[BUF_SIZE];
        bool error = false;

        // 仅仅是为了读取一次client发来的数据--仅一次--线程读完就退出本次任务
        while (1) {
            memset(buf, 0, sizeof(buf));
            int recv_len = ::recv(client_fd, buf, sizeof(buf), 0);
            if (recv_len == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("EWOULDBLOCK error\n");
                    break;
                } else {
                    printf("recv error, client %d disconnect\n", client_fd);
                    // 释放client_fd
                    releaseClient(client_fd);
                    error = true;
                    break;
                }

            } else if (recv == 0) {
                printf("client %d closed\n", client_fd);
                releaseClient(client_fd);
                error = true;
                break;
            }

            client_msg += buf;
        }

        if (error)
            continue;

        std::cout << "client " << client_fd << " msg: " << client_msg << std::endl;

        // 将消息加上时间标签后发回
        //将消息加上时间标签后发回
        time_t now = time(NULL);
        struct tm* nowstr = localtime(&now);
        std::ostringstream ostimestr;
        ostimestr << "[" << nowstr->tm_year + 1900 << "-"
                  << std::setw(2) << std::setfill('0') << nowstr->tm_mon + 1 << "-"
                  << std::setw(2) << std::setfill('0') << nowstr->tm_mday << " "
                  << std::setw(2) << std::setfill('0') << nowstr->tm_hour << ":"
                  << std::setw(2) << std::setfill('0') << nowstr->tm_min << ":"
                  << std::setw(2) << std::setfill('0') << nowstr->tm_sec << "]server reply: ";

        client_msg.insert(0, ostimestr.str());

        while (1) {
            int send_len = ::send(client_fd, client_msg.c_str(), client_msg.length(), 0);
            if (send_len == -1) {
                if (errno == EWOULDBLOCK) {
                    ::sleep(10);// ET触发--等待内核发送缓冲区可写
                    continue;
                } else {
                    printf("client %d send error\n", client_fd);
                    releaseClient(client_fd);
                    break;
                }
            }

            std::cout << "send: " << client_msg << std::endl;
            client_msg.erase(0, send_len);

            if (client_msg.empty())
                break;
        }
    }

    return NULL;

};

void daemonRun();

int main(int argc, char* argv[]) {
    int port = 0;
    int ch;

    while ((ch = getopt(argc, argv, "p:d")) != -1) {
        switch (ch) {
            case 'd':
                break;
            case 'p':
                port = atol(optarg);
                printf("port: %d\n", port);
                break;
        }
    }

    if (!createServerListener("127.0.0.1", port)) {
        printf("Unable to create listen server: ip = 127.0.0.1, port = %d\n", port);
        exit(-1);
    }

    // 设置信号处理
    signal(SIGCHLD, SIG_DFL);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGINT,destroy);
    signal(SIGKILL, destroy);
    signal(SIGTERM, destroy);

    ::pthread_cond_init(&accept_cond, NULL);
    ::pthread_mutex_init(&accept_mutex, NULL);

    ::pthread_cond_init(&client_cond, NULL);
    ::pthread_mutex_init(&client_mutex, NULL);

    // 创建accept接收线程
    ::pthread_create(&accept_id, NULL, acceptThreadFunc, NULL);
    // 创建worker普通工作线程
    for (int i = 0; i < WORKER_THREAD_NUM; i++)
        ::pthread_create(&worker_ids[i], NULL, workerThreadFunc, NULL);


    while (!stop) {
        // 发生事件的epoll_event事件集
        struct epoll_event ep_events[EPOLL_SIZE];
        int n_ready = ::epoll_wait(epoll_fd, ep_events, EPOLL_SIZE, 10);
        if (n_ready == 0)
            continue;
        else if (n_ready < 0) {
            printf("epoll_wait error\n");
            continue;
        }

        for (int i = 0; i < n_ready; i++) {
            // 通知接收线程接收新连接
            if (ep_events[i].data.fd == listen_fd)
                pthread_cond_signal(&accept_cond);
            else {
                // 加锁保护client_fd队列
                pthread_mutex_lock(&client_mutex);
                clients_list.push_back(ep_events[i].data.fd);
                pthread_mutex_unlock(&client_mutex);
                pthread_cond_signal(&client_cond);
            }
        }
    }

    printf("final line\n");
    return 0;
};


