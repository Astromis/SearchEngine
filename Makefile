# DONT FORGET updating a list of needed files after ":" and list of object files in arguments
CC=g++
CFLAGS=-c -Wall -std=c++11 -g

#INCLUDES=-I./includes/

PYTHONI = -I/usr/include/python2.7/
PYTHONL = -Xlinker -export-dynamic


all: test

test: main.o SaverData.o InvertIndex.o utils.o IndexBuilder.o
	$(CC) $(INCLUDES) main.o InvertIndex.o SaverData.o IndexBuilder.o utils.o -std=c++11 -o test -g 
    
main.o: main.cpp
	$(CC) $(CFLAGS) -c -std=c++11 main.cpp 
    
SaverData.o: SaverData.cpp
	$(CC) $(CFLAGS) SaverData.cpp	

InvertIndex.o: InvertIndex.cpp
	$(CC) $(CFLAGS) -c -std=c++11 InvertIndex.cpp

utils.o: utils.cpp
	$(CC) $(CFLAGS) -c -std=c++11 utils.cpp

IndexBuilder.o: IndexBuilder.cpp
	$(CC) $(CFLAGS) -c -std=c++11 IndexBuilder.cpp

clean:
	rm -rf *.o *.cxx *.py *.so test
	
swig:
	g++ -fPIC -c InvertIndex.cpp -o InvertIndex.o --std=c++11
	swig -c++ -python -o InvertIndex_wrap.cxx InvertIndex.i 
	g++ $(FLAGS) $(PYTHONI) -fPIC -c InvertIndex_wrap.cxx -o InvertIndex_wrap.o --std=c++11
	g++ $(PYTHONL) $(LIBFLAGS) -fPIC -shared InvertIndex.o InvertIndex_wrap.o -o _InvertIndex.so --std=c++11 
