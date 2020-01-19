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

class InvertIndex
{
    public:
        InvertIndex();
        InvertIndex(string path, string ext);
        virtual ~InvertIndex();


        bool indexing_file(string file);
        bool indexing_collection(doc_list& files);

        void threadIndexing(vector<string> &files, inverted_list &index);
        vector<int> intersect(vector<int> past, string q2);
        vector<int> MultipleIntersect(vector<string> quary);
        //interface
        vector< pair<float, string> > find(vector<string> quary);
        vector< pair<float, string> > find(string quary);

        //bool get_word_position_map(word_position_map *response, string &quary);
        //bool get_position_vector(position_vector *response, word_position_map &data, string &quary);
        //utils
        //int getdir (string ext, string dir, vector<string> &files, queue<string> &dirs);
        //bool get_dirs(const string ext, const string start_dir, vector<string> &files);

        // math
        size_t get_tfd(string  word_instance, int doc_instance);
        float get_tf(string word);
        float get_idf(string word_instance);
        float get_tf_idf(string word,int document);
        float get_smoothed_idf(string word_instance);
        float BM25(vector<string> word, int document);
        float BM25_kernel(string word, int document);

        //int ranking(string quary);
        //int ranking(vector<string> quary);
        
        void save(SaverData& saver, string dir_instance);
        void load(SaverData& saver, string dir_instance);

        void clear_index();


        vector<string> operator[](string q);

        inverted_list GetIndex() { return index; };
        map<int, string > GetN2D() { return num2doc; };
        map<string, int> GetD2N() {return doc2num; };
        long GetDocCount() { return document_count; };
        map<int, float> GetD2L() { return doc_length; }
        float GetAverLen() { return average_doc_length; }

        void SetIndex(inverted_list& list) {index = list;}
        //void SetPathfolder(string path) {pathfolder = path;}
        void SetN2D(map<int, string> n2d) {num2doc = n2d;}
        void SetD2N(map<string, int> d2n) {doc2num = d2n;}
        void SetD2L(map<int, float> d2l) {doc_length = d2l;}
        void SetDocCount(long dc) {document_count = dc;}
        void SetAvrDocLen(float adl) {average_doc_length = adl;}

    protected:


    private:
        // inverted index - common data structure
        inverted_list index;
        // path at the begging of building of index
        //string pathfolder;
        // file extentions that needs to consider
        //string extention;
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
};


#endif // INVERTINDEX_H
