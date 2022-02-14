#include "nio_event_loop.h"

static void handle_accept(IO *io) {
    sockaddr client_addr;
    socklen_t client_len;
    EventLoop *loop = io->loop;

    int client_fd = accept(io->get_fd(),&client_addr,&client_len);
    if (client_fd < 0) {
        return;
    }   

    IO *client_io = loop->get_io(client_fd,SOCKET_EVENT);

    if (io->accept_cb != nullptr) {
        io->accept_cb(client_io);
    }
}

static void handle_read(IO *io) {
    int fd = io->get_fd();
    char buf[4096] = {0};
    
    ssize_t n = read(fd,buf,4096);
    if (n < 0) {
        return;
    }

    if (io->read_cb != nullptr) {
        io->read_cb(io,buf,n);
    }
}

static void handle_events(IO *io) {
    if ((io->events | READ_EVENT) && io->is_accept()) {
        handle_accept(io);
    }

    if ((io->events | READ_EVENT) && !io->is_accept()) {
        handle_read(io);
    }
}

int NIOLoop::accept_io(IO *io,std::function<void (IO *io)> cb) {
    io->set_accept(1);
    io->set_accept_cb(cb);
    io->events |= READ_EVENT;
    add_io(io,handle_events);
    return this->event_context->add_raw_event(io);
}

int NIOLoop::read_io(IO *io,std::function<void (IO *io,char *buf, ssize_t size)> cb) {
    io->events |= READ_EVENT;
    io->set_read_cb(cb);
    add_io(io,handle_events);
    return this->event_context->add_raw_event(io);
}

NIOLoop::NIOLoop() {
    this->event_context = std::unique_ptr<EpollEventContext>(new EpollEventContext(this));
    this->set_is_stoped(false);
}

NIOLoop::~NIOLoop(){}

int NIOLoop::init(){
    return this->event_context->init();
}


void NIOLoop::start() {
    while (true) {
        // check the is_stoped flag
        if (this->get_is_stoped()) {
            return;
        }

        sleep(1);
        //std::cout << "schedule events" << std::endl;
        
        if (this->event_context->schedule_raw_events() < 0) {
            return;
        }

        // handle pendings
        while (!this->pending_io_queue.empty()) {
            IO *io = this->pending_io_queue.front();

            std::cout << io->get_fd() << ":" <<   (io->cb == nullptr) << std::endl;

            if (io->cb != nullptr) {
                io->cb(io);
            }

            this->pending_io_queue.pop();
        }
    }
}