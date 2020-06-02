CC=g++
CFLAGS= -Wall -std=c++11 -g 

all: test

test: main.o StemmerPorter.o 
	$(CC) $(CFLAGS) main.o StemmerPorter.o -o test
    
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp 
	
StemmerPorter.o: StemmerPorter.cpp
	$(CC) $(CFLAGS) -c StemmerPorter.cpp 
	
clean:
	rm -rf *.o *.cxx *.py *.so stem
