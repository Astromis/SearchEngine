CC=g++
CFLAGS=-c -Wall -std=c++11
all: test

test: main.o InvertIndex.o
	$(CC) main.o InvertIndex.o -std=c++11 -o test
    
main.o: main.cpp
	$(CC) $(CFLAGS) -c -std=c++11 main.cpp
    
InvertIndex.o: InvertIndex.cpp
	$(CC) $(CFLAGS) -c -std=c++11 InvertIndex.cpp

clean:
	rm -rf *.o test
