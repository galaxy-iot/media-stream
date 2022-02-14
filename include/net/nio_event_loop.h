#ifndef NIO_EVENT_LOOP_H
#define NIO_EVENT_LOOP_H

#include "event.h"
#include "epoll_event_context.h"
#include <memory>
#include <iostream>

class NIOLoop : public EventLoop {
    public:
        void start();
        int init();
        
        // io handler
        int accept_io(IO* io,std::function<void (IO *io)>);   
        int connect(IO* io);
        int read_io (IO* io,std::function<void (IO *io,char *buf, ssize_t size)>);
        int write (IO* io, const void* buf, int len);
        int close (IO* io);

        ~NIOLoop();
        NIOLoop();
};

#endif