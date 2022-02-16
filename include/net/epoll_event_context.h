#ifndef EPOLL_EVENT_CONTEXT_H
#define EPOLL_EVENT_CONTEXT_H

#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <queue>

#include "event.h"

class EpollEventContext : public EventContext {
    private:
        int                 epfd;
        EventLoop           *loop;
    public:
        EpollEventContext(EventLoop *loop);
        ~EpollEventContext();

        int add_raw_event(IO *event, int events);
        int remove_raw_event(IO *event,int events);
        int schedule_raw_events();
        int init();
};

#endif