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
        int write_io (IO* io, const void* buf, int len,std::function<void (IO *io)>);
        int close (IO* io);

        int add_event(IO *io, int event);
        int remove_event(IO *io, int event);
        
        ~NIOLoop();
        NIOLoop();
};

#endif