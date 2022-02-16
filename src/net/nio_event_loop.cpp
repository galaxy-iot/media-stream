#include "nio_event_loop.h"

static void handle_accept(IO *io) {
    sockaddr client_addr;
    socklen_t client_len;
    EventLoop *loop = io->get_loop();

    int client_fd = accept(io->get_fd(),&client_addr,&client_len);
    if (client_fd < 0) {
        return;
    }   

    IO *client_io = loop->get_io(client_fd,SOCKET_EVENT);

    if (io->get_accept_cb() != nullptr) {
        io->get_accept_cb()(client_io);
    }
}

static void handle_read(IO *io) {
    int fd = io->get_fd();
    char buf[4096] = {0};
    EventLoop *loop = io->get_loop();

    ssize_t n = read(fd,buf,4096);
    if (n <= 0) {
        close(fd);
        io->reset();
        loop->remove_event(io,READ_WRITE_EVENT);
        return;
    }

    if (io->get_read_cb() != nullptr) {
        io->get_read_cb()(io,buf,n);
    }
}

static void handle_events(IO *io) {
    int events = io->get_events();

    if ((events | READ_EVENT) && io->is_accept()) {
        handle_accept(io);
    }

    if ((events | READ_EVENT) && !io->is_accept()) {
        handle_read(io);
    }
}

int NIOLoop::accept_io(IO *io,std::function<void (IO *io)> cb) {
    io->set_accept_cb(cb);
    add_io(io,handle_events);
    return this->event_context->add_raw_event(io, READ_EVENT);
}

int NIOLoop::read_io(IO *io,std::function<void (IO *io,char *buf, ssize_t size)> cb) {
    io->set_read_cb(cb);
    add_io(io,handle_events);
    return this->event_context->add_raw_event(io,WRITE_EVENT);
}

int NIOLoop::write_io(IO* io, const void* buf, int len) {
    int fd = io->get_fd();
    return write(fd,buf,len);
}

NIOLoop::NIOLoop() {
    this->event_context = std::unique_ptr<EpollEventContext>(new EpollEventContext(this));
    this->set_is_stoped(false);
}

NIOLoop::~NIOLoop(){}

int NIOLoop::init(){
    return this->event_context->init();
}

int NIOLoop::add_event(IO *io, int event) {


    return 0;
}

int NIOLoop::remove_event(IO *io, int event) {

    return 0;
}

void NIOLoop::start() {
    while (true) {
        // check the is_stoped flag
        if (this->get_is_stoped()) {
            return;
        }

        if (this->event_context->schedule_raw_events() < 0) {
            return;
        }

        // handle pendings
        while (!this->pending_io_queue.empty()) {
            IO *io = this->pending_io_queue.front();

            if (io->get_cb() != nullptr) {
                io->get_cb()(io);
            }

            this->pending_io_queue.pop();
        }
    }
}