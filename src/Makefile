#CC = g++
CC = clang++
CFLAGS = -g -Wall -std=c++11
TARGET = portscan
SRC_FILES = $(wildcard *.cpp)
OBJS = ${SRC_FILES:.c=.o}

all:
	$(CC) $(CFLAGS) -c $(SRC_FILES)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)
