# DONT FORGET updating a list of needed files after ":" and list of object files in arguments
CC=g++
CFLAGS=-c -Wall -std=c++17 -g 

#INCLUDES=-I./includes/

PYTHONI = -I/usr/include/python2.7/
PYTHONL = -Xlinker -export-dynamic


all: test

test: main.o SaverData.o InvertIndex.o utils.o IndexBuilder.o
	$(CC) $(INCLUDES) main.o InvertIndex.o SaverData.o IndexBuilder.o utils.o -std=c++17 -o test -g -lstdc++fs
    
main.o: main.cpp
	$(CC) $(CFLAGS) -c -std=c++17 main.cpp 
    
SaverData.o: SaverData.cpp
	$(CC) $(CFLAGS) SaverData.cpp	-lstdc++fs

InvertIndex.o: InvertIndex.cpp
	$(CC) $(CFLAGS) -c -std=c++17 InvertIndex.cpp

utils.o: utils.cpp
	$(CC) $(CFLAGS) -c -std=c++17 utils.cpp

IndexBuilder.o: IndexBuilder.cpp
	$(CC) $(CFLAGS) -c -std=c++17 IndexBuilder.cpp

clean:
	rm -rf *.o *.cxx *.py *.so test
	
swig:
	g++ -fPIC -c InvertIndex.cpp -o InvertIndex.o --std=c++17
	swig -c++ -python -o InvertIndex_wrap.cxx InvertIndex.i 
	g++ $(FLAGS) $(PYTHONI) -fPIC -c InvertIndex_wrap.cxx -o InvertIndex_wrap.o --std=c++17
	g++ $(PYTHONL) $(LIBFLAGS) -fPIC -shared InvertIndex.o InvertIndex_wrap.o -o _InvertIndex.so --std=c++17 
