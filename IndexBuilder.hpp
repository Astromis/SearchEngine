#include <vector>
#include <string>
#include "InvertIndex.h"
#include "SaverData.h"
#include <sys/sysinfo.h>



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
}
    
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