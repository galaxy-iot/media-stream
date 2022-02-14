#ifndef EVENT_H
#define EVENT_H

#include <inttypes.h> 
#include <functional>

#include <memory>
#include <unordered_map>
#include <queue>

class IO;

#include "socket.h"
#include "event_context.h"

enum EventType {
    SOCKET_EVENT
};

enum IOType {
    READ_EVENT = 0x0001 << 0,
    WRITE_EVENT = 0x0001 << 1,
    READ_WRITE_EVENT = READ_EVENT | WRITE_EVENT
};

class EventLoop {
    protected:
        std::unique_ptr<EventContext> event_context;
        bool         is_stoped;
        std::unordered_map<int, IO*> io_map;
        std::queue<IO *> pending_io_queue;
    public:
        EventLoop() : is_stoped(false) {};
        virtual ~EventLoop() {};
        virtual int accept_io(IO* io,std::function<void (IO *io)>){return 0;};
        virtual int read_io (IO* io, std::function<void (IO *io,char *buf, ssize_t size)>) { return 0;};
        //virtual int connect(IO* io)  = 0;
        //virtual int write (IO* io, const void* buf, int len) = 0;
        //virtual int close (IO* io) = 0;
        virtual void start() {};
        virtual int init() {return 0;};
        IO *get_io(int fd, EventType event_type);
        void add_io(IO *io, std::function<void (IO *io)> cb);
        void destory_io(int fd);
        void set_is_stoped(bool is_stoped);
        bool get_is_stoped();
        void append_pending_io(IO *io);
};

class IO {
    public:
        int fd;
        uint32_t  accept :1;
        uint32_t  read   :1;
        uint32_t  write  :1;

        EventType event_type;
        int       events;
        std::function<void (IO *io)> accept_cb;
        std::function<void (IO *io, char *buf, ssize_t size)> read_cb;
        std::function<void (IO *io)> cb;
        EventLoop *loop;
    public:
        IO();
        IO(EventType event_type, EventLoop *loop);
        IO(int fd,EventType event_type, EventLoop *loop);
        ~IO();

        void set_accept(uint8_t accept);
        void set_fd(int fd);
        bool is_accept();
        int get_fd();
        void set_accept_cb(std::function<void (IO *io)> accept_cb);
        void set_read_cb(std::function<void (IO *io,char *buf, ssize_t size)> read_cb);
        void set_read_event();
        void set_write_event();
};

#endif