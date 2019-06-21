#include <vector>
#include <string>
#include "InvertIndex.h"
#include "SaverData.h"


class IndexBuilder
{
    IndexBuilder();
    IndexBuilder(int thread_num, string start_path, string ext);

    ~IndexBuilder();
    private:
    int thread_count;
    vector<InvertIndex> index_vector;
    vector<thread> threads;
    doc_list files;
    
    string start_path;
    string ext;

    bool get_dirs(const string ext, const string start_dir, vector<string> &files);
    int getdir(const string ext, const string dir, vector<string> &files, queue<string> &dirs);
    
    public:

    void threadIndexing(vector<string> &files, inverted_list &index);
    bool build_index(InvertIndex idx);

};