#include <vector>
#include <string>
#include "InvertedIndex.h"
#include "SaverData.h"
#include <sys/sysinfo.h>
#include <fstream>
#include "contrib/Stemming-ru/source/StemmerPorter.h"
#include "preprocessing.hpp"
    
class IndexBuilder
{
    private:
    Preprocessing preprocessor;
    int thread_count;
    vector<InvertedIndex> index_vector;
    vector<thread> threads;
    doc_list collection;
    map<string, int> file_sizes;
    //BinarySaverData saver;
    string start_path;
    string ext;


    bool get_dirs(const string ext, const string start_dir, vector<string> &files);
    int getdir(const string ext, const string dir, vector<string> &files, queue<string> &dirs);
    bool build_index(InvertIndex* idx, doc_list files);
    
    //bool indexing_file(string file);
    doc_list form_the_doclist();
    bool _indexing_collection(doc_list& files, InvertedIndex& index);

    public:
    IndexBuilder();
    IndexBuilder(string start_path, Preprocessing& preproc, string ext = "",  int thread_num=1 );
    void set_start_path(string start_path);
    ~IndexBuilder();
    
    bool build_index_from_collection(InvertIndex* idx);
    bool indexing_file(string file, InvertedIndex& index);
    bool indexing_collection(InvertedIndex& index);


    void BSBITest();
    void threadIndexing(doc_list &files, inverted_list &index);
    bool is_collection_empty();
    

};

word_position_map position_map_merge(word_position_map m1, word_position_map m2);