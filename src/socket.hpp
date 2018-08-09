#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>



class Socket 
{
    public:
        Socket(const char* address, const int port)
        {
            if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
            {
                perror("Failed to create socket\n");
                exit(1);
            }

            memset(&serv_addr, '0', sizeof(serv_addr));

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(port);

            if(inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
            {
                perror("Invalid address\n");
                exit(1);
            }
        }

        bool socket_connect(void);
    private:
        struct sockaddr_in serv_addr;
        int sock;
};

#endif
