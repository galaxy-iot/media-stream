#include <iostream>
#include <vector>
#include "socket.h"
#include "event.h"
#include "nio_event_loop.h"
#include "tcp_server.h"

void on_close(IO *io) {

}

void on_read(IO *io,char *buf, ssize_t size) {
    std::cout << buf << std::endl;
    io->get_loop()->write_io(io,buf,size,nullptr);
}

void on_accept(IO* io) {
    std::cout << "on accept" << std::endl;
    io->get_loop()->read_io(io,on_read);
}

int main(){
    TCPServer s(8080,on_accept);
    s.start();
}