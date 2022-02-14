#include <unistd.h>
#include "event.h"

//IO defination
IO::IO(){};

IO::IO(EventType event_type, EventLoop *loop) : event_type(event_type), loop(loop){}

IO::IO(int fd,EventType event_type,EventLoop *loop) : event_type(event_type), loop(loop) {
    this->fd = fd;
}

IO::~IO() {
    close(fd);
}

void IO::set_accept(uint8_t accept) {
    this->accept = accept;
}

void IO::set_fd(int fd) {
    this->fd = fd;
}

bool IO::is_accept() {
    return this->accept == 1;
}

int IO::get_fd() {
    return this->fd;
}

void IO::set_accept_cb(std::function<void (IO *io)> accept_cb) {
    this->accept_cb = accept_cb;
}

void IO::set_read_cb(std::function<void (IO *io,char *buf, ssize_t size)> read_cb) {
    this->read_cb = read_cb;
}

void IO::set_read_event(){
    this->events |= READ_EVENT;
}

void IO::set_write_event(){
    this->events |= WRITE_EVENT;
}

// Event loop definations
IO* EventLoop::get_io(int fd,EventType event_type) {
    // io exist, return it
    if (io_map.find(fd) != io_map.end()) {
        return io_map[fd];
    }

    io_map[fd] = new IO(fd,event_type,this);

    return io_map[fd];
};

void EventLoop::add_io(IO *io, std::function<void (IO *io)> cb) {
    io->cb = cb;
    int fd = io->get_fd();
    io_map[fd] = io;
}

void EventLoop::destory_io(int fd) {
    if (io_map.find(fd) != io_map.end()) {
        close(fd);
        io_map.erase(fd);
    }
};

void EventLoop::set_is_stoped(bool is_stoped) {
    this->is_stoped = is_stoped;
}

bool EventLoop::get_is_stoped() {
    return this->is_stoped;
}

void EventLoop::append_pending_io(IO *io) {
    this->pending_io_queue.push(io);
}