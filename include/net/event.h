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
    public:
        std::unique_ptr<EventContext> event_context;
        bool         is_stoped;
        std::unordered_map<int, IO*> io_map;
        std::queue<IO *> pending_io_queue;
    public:
        EventLoop() : is_stoped(false) {};
        virtual ~EventLoop() {};
        virtual int accept_io(IO* io,std::function<void (IO *io)>){return 0;};
        virtual int read_io (IO* io, std::function<void (IO *io,char *buf, ssize_t size)>) { return 0;};
        virtual int write_io (IO* io, const void* buf, int len,std::function<void (IO *io)>) { return 0;};
        virtual void start() {};
        virtual int init() {return 0;};
        
        IO *get_io(int fd, EventType event_type);
        void add_io(IO *io, std::function<void (IO *io)> cb);
        void destory_io(int fd);
        void set_is_stoped(bool is_stoped);
        bool get_is_stoped();
        void append_pending_io(IO *io);

        int add_event(IO *io, int events);
        int remove_event(IO *io, int events);
};

class IO {
    private:
        int fd;
        uint32_t  accept  :1;
        uint32_t  read    :1;
        uint32_t  write   :1;
        uint32_t  connect :1;
        
        EventType event_type;
        // real io events
        int       events;
        std::function<void (IO *io)> accept_cb;
        std::function<void (IO *io, char *buf, ssize_t size)> read_cb;
        std::function<void (IO *io)> cb;
        std::function<void (IO *io)> write_cb;

        EventLoop *loop;

        char *read_buf;
        int read_buf_len;

        char *write_buf;
        int write_buf_len;
    public:
        IO();
        IO(EventType event_type, EventLoop *loop);
        IO(int fd,EventType event_type, EventLoop *loop);
        ~IO();

        void set_fd(int fd);

        bool is_accept();
        bool is_read();
        bool is_write();
        bool is_connect();

        int get_fd();

        void set_accept_cb(std::function<void (IO *io)> accept_cb);
        std::function<void (IO *io)> get_accept_cb();

        void set_read_cb(std::function<void (IO *io,char *buf, ssize_t size)> read_cb);
        std::function<void (IO *io,char *buf, ssize_t size)> get_read_cb();

        void set_cb(std::function<void (IO *io)> cb);
        std::function<void (IO *io)> get_cb();

        void set_write_cb(std::function<void (IO *io)> write_cb);
        std::function<void (IO *io)> get_write_cb();

        void set_events(int events);
        void clear_events(int events);
        int get_events();

        EventLoop *get_loop();

        void reset();
};

#endif