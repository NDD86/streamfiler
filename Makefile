SHELL = /bin/sh

OBJS = argumentreader.o filesavemanager.o filesaver.o posixserver.o
CFLAGS = -Wall -g -std=c++17 -O2
CC = g++
INCLUDE =
LIBS = -lpthread -lstdc++fs

streamfiler:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ main.cpp ${OBJS} ${LIBS}

clean:
	-rm -f *.o core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<