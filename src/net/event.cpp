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

void IO::set_fd(int fd) {
    this->fd = fd;
}

bool IO::is_accept() {
    return this->accept == 1;
}

bool IO::is_read() {
    return this->read == 1;
}

bool IO::is_write() {
    return this->write == 1;
}

bool IO::is_connect() {
    return this->connect == 1;
}

int IO::get_fd() {
    return this->fd;
}

void IO::set_accept_cb(std::function<void (IO *io)> accept_cb) {
    this->accept = 1;
    this->accept_cb = accept_cb;
}

std::function<void (IO *io)> IO::get_accept_cb() {
    return this->accept_cb;
}

void IO::set_write_cb(std::function<void (IO *io)> write_cb) {
    this->write = 1;
    this->write_cb = write_cb;
}

void IO::set_read_cb(std::function<void (IO *io,char *buf, ssize_t size)> read_cb) {
    this->read = 1;
    this->read_cb = read_cb;
}

std::function<void (IO *io,char *buf, ssize_t size)> IO::get_read_cb() {
    return this->read_cb;
}

std::function<void (IO *io)> IO::get_cb() {
    return this->cb;
}

EventLoop * IO::get_loop() {
    return this->loop;
}

void IO::set_cb(std::function<void (IO *io)> cb){
    this->cb = cb;
}

void IO::set_events(int events) {
    this->events |= events;
}

void IO::clear_events(int events) {
    this->events &= ~events;
}

void IO::reset() {
    this->events = 0;
    this->accept = 0;
    this->read = 0;
    this->write = 0;
    this->connect = 0;

    if (this->read_buf != nullptr) {
        this->read_buf->len = 0;
    }
    
    if (this->write_buf != nullptr) {
        this->write_buf->len = 0;
    }
}

int IO::get_events() {
    return this->events;
}

Buffer *IO::get_read_buffer() {
    if (this->read_buf != nullptr) {
        return this->read_buf;
    }

    this->read_buf = new Buffer();
    return this->read_buf;
}

Buffer *IO::get_write_buffer(){
    if (this->write_buf != nullptr) {
        return this->write_buf;
    }

    this->write_buf = new Buffer();
    return this->write_buf;
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
    io->set_cb(cb);
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

int EventLoop::add_event(IO *io,int events) {
    return this->event_context->add_raw_event(io,events);
}

int EventLoop::remove_event(IO *io, int events) {
    return this->event_context->remove_raw_event(io,events);
}