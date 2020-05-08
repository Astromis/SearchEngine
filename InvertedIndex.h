#ifndef INVERTINDEX_H
#define INVERTINDEX_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <queue>
#include <thread>
#include <mutex>
#include <math.h>


using namespace std;



// this vector contains an order number of word in text
typedef vector<int> position_vector;
// this map contains documents (its ID) as a key and position vectors as a value
typedef map<int, position_vector> word_position_map;
// this is finally inverted list
typedef map <string, word_position_map > inverted_list;
typedef vector<string> doc_list;

class SaverData;

struct InvertedIndex
{
        // inverted index - common data structure
        inverted_list index;
        static int doc_id;
        // tables that defines relations between documents and its indeces
        map<int, string > num2doc;
        map<string, int> doc2num;
        // amout of document in the index
        long document_count;
        // table of document length
        map<int, float> doc_length;
        // average document length (need for BM25)
        float average_doc_length;

        void save(SaverData& saver, string dir_instance);
        void load(SaverData& saver, string dir_instance);
        void clear_index();
};


#endif // INVERTINDEX_H
