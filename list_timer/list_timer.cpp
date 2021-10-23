#include <stdio.h>
#include <time.h>
#include "list_timer.h"

timer_list::timer_list()
{
    head = nullptr;
    tail = nullptr;
}

timer_list::~timer_list()
{
    // 链表销毁时，删除所有节点
    timer_node* tmp = head;// tmp指向链表头结点
    while (tmp)
    {
        head = tmp->next;
        delete tmp;
        tmp = head;// tmp重新指向新head
    }
}

void timer_list::add_timer(timer_node* timer)
{
    if (!timer)
        return;

    // 插入的timer为链表第一个timer_node
    if (!head)
    {
        head = tail = timer;
        return;
    }

    // 升序插入链表--执行一次插入排序
    if (timer->expire < head->expire)
    {
        // 双向链表的插入操作
        timer->next = head->next;
        head->prev = timer;
        head = head;
        return;
    }

    // 递归调用，直到找到比timer->expire大的第一个节点
    add_timer(timer, head);
}

// private add_timer()
void timer_list::add_timer(timer_node *timer, timer_node *last_timer)
{
    // 将timer插入到第一个tmp->expire > timer->expire的节点之前
    timer_node* prev = last_timer;
    timer_node* tmp = prev->next;
    while (tmp)
    {
        if (timer->expire < tmp->expire)
        {
            prev->next = timer;
            timer->next = tmp;
            tmp->prev = timer;
            timer->prev = prev;
            break;
        }

        prev = tmp;
        tmp = tmp->next;
    }

    // 遍历完整个链表仍然没有找到满足tmp->expire > timer->expire的节点--将timer插入到链表尾
    if (!tmp)
    {
        prev->next = timer;
        timer->prev = prev;
        timer->next = nullptr;
        tail = timer;
    }
}

// 某个定时器任务发生变化时，调整该定时器的expire，然后将该timer_node向链表尾部移动
void timer_list::adjust_timer(timer_node* timer)
{
    if (!timer)
        return;

    timer_node* tmp = timer->next;
    // 若timer位于链表尾部或经增大后的timer->expire仍然小于timer->next->expire--则不调整，仍留在原位置
    if (!tmp || (timer->expire < tmp->expire))
        return;

    // 若timer为头结点，则将该timer从链表中取出，从并重新插入链表
    if (timer == head)
    {
        head = head->next;
        head->prev = nullptr;
        timer->next = nullptr;// ???
        // 重新使用add_timer()将timer插入到链表合适的位置
        add_timer(timer, head);
    }
    // 若timer不是头结点，则从原链表中取出，并插入到tmp之后的节点
    else
    {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer, timer->next);
    }
}

// 将定时器timer从链表中删除
void timer_list::del_timer(timer_node* timer)
{
    if (!timer)
        return;

    // 链表中只有一个定时器timer
    if (timer == head && timer == tail)
    {
        delete timer;
        head = nullptr;
        tail = nullptr;
        return;
    }

    // timer为链表头结点
    if (timer == head)
    {
        head = head->next;
        head->prev = nullptr;
        delete timer;
        return;
    }

    // timer为尾节点
    if (timer == tail)
    {
        tail = tail->prev;
        tail->next = nullptr;
        delete timer;
        return;
    }

    // timer位于链表中间
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;

    return;
}

// 脉搏函数--每次收到SIGALRM信号便触发一次
// 1 链表中没有timer
// 2 链表中有timer--查看是否有超时timer--删除之--并触发SIGALRM信号处理函数
void timer_list::tick(int epollfd)
{
    if (!head)
    {
        printf("链表为空，当前没有客户连接\n");
        return;
    }

    printf("timer tick\n");
    time_t cur_time = time(nullptr);// 获取系统当前时间
    timer_node* tmp = head;
    // 从头结点开始依次遍历每个timer，直到找到一个超时的timer--cur_timer > timer->expire--实际只看表头结点即可
    while (tmp)
    {
        // 未超时
        if (cur_time < tmp->expire)
            break;

        // 超时--执行SIGALRM信号处理函数--关闭sockfd，并从epollfd例程空间中删除该sockfd
        tmp->cb_func(tmp->user_data, epollfd);
        // 执行完任务后，经其从timer_list中删除
        head = tmp->next;
        if (head)
            head->prev = nullptr;

        delete tmp;
        tmp = head;// 继续遍历下一个
    }

}
