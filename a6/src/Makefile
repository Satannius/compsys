CC = gcc
CFLAGS =-Wall -Wextra -pedantic -std=gnu11
LD_FLAGS = -lpthread

all : peer name_server

echosingle :
	$(CC) $(CFLAGS) -c echo.c -o echo.o
	$(CC) $(CFLAGS) echoclient.c -o echoclient csapp.o $(LD_FLAGS)
	$(CC) $(CFLAGS) echoserveri.c -o echoserveri csapp.o echo.o $(LD_FLAGS)

rebuild : clean all

csapp.o : csapp.c csapp.h
	$(CC) $(CFLAGS) -c $< -o $@

peer : peer.c peer.h csapp.o
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

name_server : name_server.c name_server.h csapp.o
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

clean :
	rm -rf *.o peer name_server

clean_echosingle :
	rm -rf *.o echoclient echoserveri

../src.zip :
	make clean
	cd .. && zip src.zip -r src
