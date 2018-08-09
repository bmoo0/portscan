#include "socket.hpp"

bool Socket::socket_connect() {
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        return false;
    }
    return true;
}
