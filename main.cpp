#include <iostream>
#include <vector>
#include "socket.h"
#include "event.h"
#include "nio_event_loop.h"

void on_close(IO *io) {

}

void on_read(IO *io,char *buf, ssize_t size) {
    std::cout << buf << std::endl;
    io->get_loop()->write_io(io,buf,size);
}

void on_accept(IO* io) {
    std::cout << "on accept" << std::endl;
    io->get_loop()->read_io(io,on_read);
}

int main(){
    Listener l("0.0.0.0",8080);
    int serverFd = l.listen();
    if (serverFd < 0) {
        std::cout << "init socket failed" << std::endl;
        exit(-1);
    }

    NIOLoop loop;
    loop.init();

    IO e(serverFd,SOCKET_EVENT,&loop);
    int ret = loop.accept_io(&e,on_accept);
    if (ret < 0) {
        std::cout << "accept failed" << std::endl;
        exit(-1);
    }

    loop.start();
}