# DONT FORGET updating a list of needed files after ":" and list of object files in arguments
CC=g++
CFLAGS=-c -Wall -std=c++17 -g 

#INCLUDES=-I./includes/

PYTHONI = -I/usr/include/python2.7/
PYTHONL = -Xlinker -export-dynamic


all: test

test: main.o SaverData.o InvertedIndex.o utils.o IndexBuilder.o Ranker.o IndexInterface.o StemmerPorter.o Preprocessing.o
	$(CC) $(INCLUDES) main.o InvertedIndex.o SaverData.o IndexBuilder.o utils.o Ranker.o IndexInterface.o StemmerPorter.o  preprocessing.o -std=c++17 -o test -g -lstdc++fs
    
main.o: main.cpp
	$(CC) $(CFLAGS) -c -std=c++17 main.cpp 
    
SaverData.o: SaverData.cpp
	$(CC) $(CFLAGS) SaverData.cpp	-lstdc++fs

InvertedIndex.o: InvertedIndex.cpp
	$(CC) $(CFLAGS) -c -std=c++17 InvertedIndex.cpp

IndexInterface.o: IndexInterface.cpp
	$(CC) $(CFLAGS) -c -std=c++17 IndexInterface.cpp

utils.o: utils.cpp
	$(CC) $(CFLAGS) -c -std=c++17 utils.cpp

IndexBuilder.o: IndexBuilder.cpp
	$(CC) $(CFLAGS) -c -std=c++17 IndexBuilder.cpp

Ranker.o: Ranker.cpp
	$(CC) $(CFLAGS) -c -std=c++17 Ranker.cpp

StemmerPorter.o: contrib/Stemming-ru/source/StemmerPorter.cpp
	$(CC) $(CFLAGS) -c -std=c++17 contrib/Stemming-ru/source/StemmerPorter.cpp

Preprocessing.o: preprocessing.cpp
	$(CC) $(CFLAGS) -c -std=c++17 preprocessing.cpp

clean:
	rm -rf *.o *.cxx *.py *.so test
	
swig:
	g++ -fPIC -c InvertIndex.cpp -o InvertIndex.o --std=c++17
	swig -c++ -python -o InvertIndex_wrap.cxx InvertIndex.i 
	g++ $(FLAGS) $(PYTHONI) -fPIC -c InvertIndex_wrap.cxx -o InvertIndex_wrap.o --std=c++17
	g++ $(PYTHONL) $(LIBFLAGS) -fPIC -shared InvertIndex.o InvertIndex_wrap.o -o _InvertIndex.so --std=c++17 
