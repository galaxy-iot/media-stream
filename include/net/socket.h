#ifndef SOCKET_H
#define SOCKET_H

#include <string>

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/types.h>

#include "event.h"

class Listener {
    private:
        std::string ip;
        uint16_t    port;
        int socker_fd;
    public:
        Listener(std::string ip, uint16_t port);
        ~Listener();

        int listen();
};

#endif