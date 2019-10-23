#include <vector>
#include <string>
#include "InvertIndex.h"
#include "SaverData.h"
#include <sys/sysinfo.h>
#include <fstream>


struct sysinfo_st 
{
    long uptime;             /* Seconds since boot */
    unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    unsigned long totalram;  /* Total usable main memory size */
    unsigned long freeram;   /* Available memory size */
    unsigned long sharedram; /* Amount of shared memory */
    unsigned long bufferram; /* Memory used by buffers */
    unsigned long totalswap; /* Total swap space size */
    unsigned long freeswap;  /* swap space still available */
    unsigned short procs;    /* Number of current processes */
    unsigned long totalhigh; /* Total high memory size */
    unsigned long freehigh;  /* Available high memory size */
    unsigned int mem_unit;   /* Memory unit size in bytes */
    char _f[20-2*sizeof(long)-sizeof(int)]; /* Padding for libc5 */
};
    
class IndexBuilder
{
    private:
    int thread_count;
    vector<InvertIndex> index_vector;
    vector<thread> threads;
    doc_list collection;
    map<string, int> file_sizes;

    string start_path;
    string ext;

    bool get_dirs(const string ext, const string start_dir, vector<string> &files);
    int getdir(const string ext, const string dir, vector<string> &files, queue<string> &dirs);
    bool build_index(InvertIndex* idx, doc_list files);
    
    public:
    IndexBuilder();
    IndexBuilder(string start_path, string ext = "", int thread_num=1);
    ~IndexBuilder();
    
    void BSBITest();
    void threadIndexing(doc_list &files, inverted_list &index);
    
    bool index_collection(InvertIndex* idx);

};