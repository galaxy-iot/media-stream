#ifndef TCP_SERVER_H
#define TCP_SERVER

#include <functional>

#include "event.h"
#include "nio_event_loop.h"


class TCPServer {
    private:
        EventLoop *loop;
        uint16_t  port;
        std::function<void (IO *io)> accept_cb;

    public:
        TCPServer();
        TCPServer(uint16_t port);
        TCPServer(uint16_t port, std::function<void (IO *io)> accept_cb);
        ~TCPServer();

        void set_accept_cb(std::function<void (IO *io)> accept_cb);
        std::function<void (IO *io)> get_accept_cb();
        int start();
        int stop();
};

#endif