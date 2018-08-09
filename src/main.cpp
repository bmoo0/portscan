#include "socket.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <string>

void usage() {
    std::cout << "usage: portscan <address> [-p <port>] [-r <range_start-range_end>]" << std::endl;
}

std::vector<std::string> split(std::string strToSplit, char delimeter) {
		std::stringstream ss(strToSplit);
		std::string item;
		std::vector<std::string> splittedStrings;

		while (std::getline(ss, item, delimeter))
		{
				splittedStrings.push_back(item);
		}
		return splittedStrings;
}

int get_range_from_arg(std::string input, int* start, int* end) {
    std::vector<std::string> vecs = split(input, '-');

    if (vecs.size() != 2) {
        return -1; // error
    }

    std::string::size_type sz;

    *start = std::stoi(vecs.front(), &sz);
    *end = std::stoi(vecs.back(), &sz);

    return 0;
}

void scan_port(const char* addr, const int* port) {
    Socket sock(addr, *port);

    //std::cout << "Port: " << *port << " on address: " << addr;
    std::cout << "|" << std::setw(4) << *port << "|";
    if(sock.socket_connect()) {
        std::cout << std::setw(6) << "open";
    } else {
        std::cout << std::setw(6) << "closed";
    }

    std::cout << "|" << std::endl;
}

void print_header(const char* addr) {
    std::cout << "Scanning server at address: " << addr << std::endl;
    std::cout << "+----+------+" << std::endl;
    std::cout << "|Port|Status|" << std::endl;
    std::cout << "+----+------+" << std::endl;
}

int main(int argc, char** argv) {
    int opt;
    int port;
    int singleFlag = 0; // single port
    int* range_start;
    int* range_end;
    char* addr;

    while((opt = getopt(argc, argv, "p:r:a:")) != -1) {
        switch(opt) {
            case 'p':
                {
                    port = atoi(optarg);
                    singleFlag = 1;
                    #ifdef _DEBUG
                    std::cout << "Port passed: " << port << std::endl;
                    #endif
                    break;
                }
            case 'r':
                {
                    if(singleFlag == 1) return 1;
                    std::string arg_str(optarg);
                    range_start = (int*)malloc(sizeof(int));
                    range_end =(int*)malloc(sizeof(int));
                    if((get_range_from_arg(arg_str, range_start, range_end)) < 0) {
                        usage();
                        return 1;
                    }
                    break;
                }
            case 'a':
                {
                    addr = optarg;
                    break;
                }
            default:
                {
                    usage();
                    break;
                }
        }
    }

    if(singleFlag == 1) {
        print_header(addr);
        scan_port(addr, &port);
        std::cout << "+----+------+" << std::endl;
    } else {
        print_header(addr);

        for(int i = *range_start; i <= *range_end; i++) {
            scan_port(addr, &i);
        }
        std::cout << "+----+------+" << std::endl;
    }
}
