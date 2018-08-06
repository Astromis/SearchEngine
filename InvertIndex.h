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

using namespace std;



// this vector contains an order number of word in text
typedef vector<int> position_vector;
// this map contains documents as a key and position vectors as a value
typedef map<string,position_vector> word_position_map;
// this is finally inverted list
typedef map <string, word_position_map > inverted_list;
typedef vector<string> doc_list;

class InvertIndex
{
    public:
        InvertIndex(string path, string ext);
        virtual ~InvertIndex();

        bool build_index();
        void threadIndexing(vector<string> &files, inverted_list &index);
        bool intersect(vector<string> &result, string q1, string q2);
        bool MultipleIntersect(vector<string> *result, vector<string> quary);
        //interface
        bool get_word_position_map(word_position_map *response, string &quary);
        bool get_position_vector(position_vector *response, word_position_map &data, string &quary);
        //utils
        int getdir (string ext, string dir, vector<string> &files, queue<string> &dirs);
        bool get_dirs(const string ext, const string start_dir, vector<string> &files);
    
        vector<string> operator[](string q);
    protected:

    private:
        inverted_list index;
        string pathfolder;
        string extention;
};

#endif // INVERTINDEX_H
