#include "list_timer.h"

#define TIMESLOT 5

extern int pipefd[2];

class timer_signal
{
public:
    timer_signal();
    ~timer_signal();

    static void get_pipefd();
    int set_non_blocking(int fd);
    void add_fd(int epollfd, int fd);
    static void sig_handler(int sig);
    void add_sig(int sig);
    void timer_handler(timer_list &timer_list, int epollfd);
    static void cb_func(client_data* user_data, int epollfd);
};