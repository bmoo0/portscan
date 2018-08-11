#include "socket.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <mutex>
#include <thread>

typedef struct port_status {
    int port;
    char status;
    char* addr;
} p_status;

std::vector<port_status> status;
std::mutex mtx;
bool isFinished = false;


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

    if (*start > *end) {
        return -1; // error
    }

    if (vecs.size() != 2) {
        return -1; // error
    }

    std::string::size_type sz;

    *start = std::stoi(vecs.front(), &sz);
    *end = std::stoi(vecs.back(), &sz);

    return 0;
}

void scan_port(p_status* pstat) {
    Socket sock(pstat->addr, pstat->port);

    if(sock.socket_connect()) {
        pstat->status = 'o';
    } else {
        pstat->status = 'o';
    }

    mtx.lock();
    status.push_back(*pstat);
    mtx.unlock();
}

bool compare_by_port(p_status& a, p_status& b) {
    return a.port < b.port;
}

void print_results(std::vector<port_status> stats) {
    std::sort(stats.begin(), stats.end(), compare_by_port);

    std::cout << std::endl << "+----+------+" << std::endl;
    std::cout << "|Port|Status|" << std::endl;
    std::cout << "+----+------+" << std::endl;

    for(std::vector<port_status>::iterator i = stats.begin(); i != stats.end();i++) {
        std::cout << "|" << std::setw(4) << i->port << "|";
        if (i->status == 'o') {
            std::cout << std::setw(6) << "open";
        } else {
            std::cout << std::setw(6) << "closed";
        }
        std::cout << "|" << std::endl;
    }

    std::cout << "+----+------+" << std::endl;
}

void print_header(const char* addr) {
    std::cout << "Scanning server at address: " << addr << std::endl;
}

void loading_screen() {
    std::cout << '-' << std::flush;
    while (!isFinished) {
        sleep(1);
        std::cout << "\b\\" << std::flush;
        sleep(1);
        std::cout << "\b|" << std::flush;
        sleep(1);
        std::cout << "\b/" << std::flush;
        sleep(1);
        std::cout << "\b-" << std::flush;
    }
    std::cout << std::flush;
}


int main(int argc, char** argv) {
    int opt;
    int singleFlag = 0; // single port
    int* range_start = (int*)malloc(sizeof(int));
    int* range_end = (int*)malloc(sizeof(int));
    char* addr;

    while((opt = getopt(argc, argv, "p:r:a:")) != -1) {
        switch(opt) {
            case 'p':
                {
                    *range_start = atoi(optarg);
                    *range_end = atoi(optarg);
                    singleFlag = 1;
                    break;
                }
            case 'r':
                {
                    if(singleFlag == 1) return 1;
                    std::string arg_str(optarg);
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

    print_header(addr);
    int num_threads = *range_end - *range_start;

    std::thread threads[num_threads];
    int count = 0;


    for(int i = *range_start; i <= *range_end; i++) {
        p_status *stat = (p_status *)malloc(sizeof(p_status));
        stat->addr = addr;
        stat->port = i;
        threads[count] = std::thread(scan_port, stat);
        count++;
    }

    std::thread loading_thread(loading_screen);

    count = 0;


    for(int i = *range_start; i <= *range_end; i++) {
        threads[count].join();
        count++;
    }

    isFinished = true;
    loading_thread.join();

    print_results(status);

}
