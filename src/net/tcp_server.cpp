#include "tcp_server.h"

TCPServer::TCPServer() : port(8080){}

TCPServer::TCPServer(uint16_t port) : port(port){}

TCPServer::TCPServer(uint16_t port,std::function<void (IO *io)> accept_cb) : port(port), accept_cb(accept_cb) {}

TCPServer::~TCPServer() {}

std::function<void (IO *io)> TCPServer::get_accept_cb() {
    return this->accept_cb;
}

void TCPServer::set_accept_cb(std::function<void (IO *io)> accept_cb) {
    this->accept_cb = accept_cb;
}

int TCPServer::start() {
    Listener l("0.0.0.0",this->port);
    int serverFd = l.listen();
    if (serverFd < 0) {
        return -1;
    }

    NIOLoop loop;
    loop.init();

    IO e(serverFd,SOCKET_EVENT,&loop);

    if (this->accept_cb == nullptr) {
        std::cout << "accept callback function is empty" << std::endl;
        return -1;
    }

    int ret = loop.accept_io(&e, this->accept_cb);
    if (ret < 0) {
        std::cout << "accept failed" << std::endl;
        exit(-1);
    }

    loop.start();
    
    return 0;
}