#include <vector>
#include <string>
#include "InvertIndex.h"
#include "SaverData.h"


class IndexBuilder
{
    private:

    bool get_dirs(const string ext, const string start_dir, vector<string> &files);
    int getdir(const string ext, const string dir, vector<string> &files, queue<string> &dirs);
    
    public:

    void threadIndexing(vector<string> &files, inverted_list &index);
    bool build_index(InvertIndex idx);

};