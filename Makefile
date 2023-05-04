CC = g++
CFLAGS = -g -std=c++11 -Wall -Werror

all: proj1

proj1: queue.o main.o pQueue.o eQueue.o jQueue.o eventQueue.o queue.h pQueue.h jQueue.h eQueue.h eventQueue.h
	$(CC) -o proj1 main.o queue.o eQueue.o pQueue.o jQueue.o eventQueue.o -lm

main.o: main.cpp pQueue.h eQueue.h jQueue.h queue.h main.h
	$(CC) $(CFLAGS) -c main.cpp pQueue.cpp eQueue.cpp jQueue.cpp queue.cpp

pQueue.o: pQueue.cpp pQueue.h queue.h
	$(CC) $(CFLAGS) -c pQueue.cpp queue.cpp

eQueue.o: eQueue.cpp eQueue.h queue.h
	$(CC) $(CFLAGS) -c eQueue.cpp queue.cpp

jQueue.o: jQueue.cpp jQueue.h queue.h
	$(CC) $(CFLAGS) -c jQueue.cpp queue.cpp

queue.o: queue.cpp queue.h
	$(CC) $(CFLAGS) -c queue.cpp

eventQueue.o: eventQueue.cpp eventQueue.h queue.h
	$(CC) $(CFLAGS) -c eventQueue.cpp

clean:
	rm -f core *.o proj1