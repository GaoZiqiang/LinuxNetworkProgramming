#ifndef LIST_TIMER
#define LIST_TIMER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 64
class timer_node;// 前向声明--供struct client_data使用

// 用户数据结构:客户端地址、文件描述符sockfd、读缓存和定时器
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUF_SIZE];
    timer_node* timer;
};

// 定时器类--链表元素/链表单元--链表节点的存储结构
class timer_node
{
public:
    // 构造器
    timer_node() : prev(nullptr), next(nullptr) {}

public:
    // 存储结构内容
    // 1 闹钟本身信息--超时时间、处理函数
    time_t expire;// 任务超时时间--绝对时间--定时器到期时间
    void (*cb_func) (client_data*, int);// 回调函数--信号处理函数
    // 2 与闹钟绑定的客户数据
    client_data* user_data;
    // 3 节点前后指针
    timer_node* prev;// 前向指针
    timer_node* next;// 后继指针
};

// 定时器链表--定时器管理类--升序、双向列表
class timer_list
{
public:
    timer_list();
    ~timer_list();

    void add_timer(timer_node* timer);
    void adjust_timer(timer_node* timer);
    void del_timer(timer_node* timer);
    // 脉搏函数--每次收到SIGALRM信号便触发一次
    void tick(int epollfd);// 待优化--epollfd可以作为全局变量--两个文件共享

private:
    // 链表的头指针和尾指针
    timer_node* head;
    timer_node* tail;

    // 重载--将timer添加到last_timer之后的链表中--供add_timer()和adjust_timer()使用
    void add_timer(timer_node* timer, timer_node* last_timer);

};
#endif