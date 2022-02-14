#include "socket.h"
#include <iostream>

Listener::Listener(std::string ip, uint16_t port) : ip(ip),port(port) {}

Listener::~Listener() {
    #ifdef OS_WIN
    WSACleanUp();
    closesocket(this->);
    #else 
    close(this->socker_fd);
    #endif
}

int Bind(std::string host, int port, int type) {
#ifdef OS_WIN
    WSAInit();
#endif
    int sockerFD = socket(AF_INET,type,0);
    if (sockerFD < 0) {
        return -1;
    }

    sockaddr_in serverSocket;
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(port);
    serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(sockerFD,(sockaddr *)&serverSocket,sizeof(sockaddr));
    if (ret < 0) {
        return ret;
    }

    return sockerFD;
}

int ListenFD(int socketFD,int size) {
    return listen(socketFD,size);
}

int Listener::listen() {
    int socket_fd = Bind(this->ip, this->port, SOCK_STREAM);
    if (socket_fd < 0) {
        std::cout << "bind failed" << std::endl;
        return socket_fd;
    }
    
    this->socker_fd = socket_fd;
    if (ListenFD(socket_fd,1024) < 0){
        return -1;
    }

    return socker_fd;
}

