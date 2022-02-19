#include "epoll_event_context.h"

EpollEventContext::EpollEventContext(EventLoop *loop){
    this->loop = loop;
}

EpollEventContext::~EpollEventContext(){
    close(this->epfd);
}

int EpollEventContext::init(){
    int fd = epoll_create(1024);    
    if (fd < 0) {
        std::cout << "epoll create failed" << std::endl;
        return -1;
    }

    this->epfd = fd;
    return 0;
}

int EpollEventContext::add_raw_event(IO *io,int events) {
    struct epoll_event ee;
    memset(&ee, 0, sizeof(ee));
    int fd = io->get_fd();
    ee.data.fd = fd;

    // pre events
    if (io->is_read()) {
        ee.events |= EPOLLIN;
    }
    
    if (io->is_write()) {
        ee.events |= EPOLLOUT;
    }

    if (events & READ_EVENT) {
        ee.events |= EPOLLIN;
    }

    if (events & WRITE_EVENT) {
        ee.events |= EPOLLOUT;
    }

    int op = (io->get_events() == 0) ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    if (epoll_ctl(this->epfd, op, fd, &ee) < 0){
        return -1;
    }

    //io->set_events(events);
    return 0;
}

int EpollEventContext::remove_raw_event(IO *io, int events) {
    int fd = io->get_fd();
    struct epoll_event ee;
    memset(&ee, 0, sizeof(ee));
    ee.data.fd = fd;

    // pre events
    if (io->is_read()) {
        ee.events |= EPOLLIN;
    }
    if (io->is_write()) {
        ee.events |= EPOLLOUT;
    }

    // now events
    if (events & READ_EVENT) {
        ee.events &= ~EPOLLIN;
    }
    if (events & WRITE_EVENT) {
        ee.events &= ~EPOLLOUT;
    }

    int op = ee.events == 0 ? EPOLL_CTL_DEL : EPOLL_CTL_MOD;

    return epoll_ctl(this->epfd,op,fd,&ee);
}

int EpollEventContext::schedule_raw_events() {
    struct epoll_event events[10];

    int ret = epoll_wait(this->epfd, events,10, 1000);
    if (ret < 0) {
        return ret;
    }

    if (ret == 0) {
        return 0;
    }

    for (int i = 0;i < ret;i++) {
        IO *io = this->loop->get_io(events[i].data.fd, SOCKET_EVENT);
        if (io != nullptr) {
            int revents = events[i].events;

            if (revents & (EPOLLIN | EPOLLHUP | EPOLLERR)) {
                io->set_events(READ_EVENT);
            }

            if (revents & (EPOLLOUT | EPOLLHUP | EPOLLERR)) {
                io->set_events(WRITE_EVENT);
            }

            this->loop->append_pending_io(io);
        }
    }

    return 0;
}

