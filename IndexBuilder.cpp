#include "IndexBuilder.hpp"
#include "SaverData.h"

IndexBuilder::IndexBuilder()
{

}

IndexBuilder::IndexBuilder(string start_path, string ext, int thread_num)
{
    thread_count = thread_num;
    start_path = start_path;
    ext = ext;
    if(start_path != "")
        get_dirs(ext, start_path, collection);
    threads.resize(thread_num);
}


IndexBuilder::~IndexBuilder()
{

}

void IndexBuilder::set_start_path(string start_path)
{
    start_path = start_path;
    get_dirs(ext, start_path, collection);

}
/**
 * @brief Create an inverted index for a given instane of InvertedIndex class
 * Starting with entry point, go through the all directores 
 * and colect tokens.
 * @return True if index has been built successfully
 */

/* bool IndexBuilder::build_index(InvertIndex* idx, doc_list files)
{
    idx->indexing_collection(files);
} */

bool IndexBuilder::build_index_from_collection(InvertIndex* idx)
{
    idx->indexing_collection(collection);
}

void IndexBuilder::threadIndexing(vector<string> &files, inverted_list &index)
{
    vector<string> a(files.begin(), files.begin() + files.size()/ 2);
    vector<string> b(files.begin() + files.size()/ 2, files.end());

/*     thread thread1(build_index, ref(a));
    thread thread2(build_index, ref(b));
    thread1.join();
    thread2.join(); */
}

ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

/**
 * @brief Gef files and dirs in one dir
 * @param [in] ext Extention of file that need to collect
 * @param [in] dir Dir path
 * @param [in] files Files, which contains in dir
 * @param [in] dirs Dirs,  which contains in dr
 * @return 0 If everithing is ok
 */
int IndexBuilder::getdir (const string ext, const string dir, vector<string> &files, queue<string> &dirs)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if(dirp->d_type == DT_DIR && string(dirp->d_name) != "." && string(dirp->d_name) != "..")

            dirs.push(dir+"/"+string(dirp->d_name));
        else
        //This condition is for the exact file extension. If it is not present, all file names will be appended to a vector.
        if(ext != "")
        {
            if(string(dirp->d_name).find(ext+"\0") != -1)
            {
                string filename = dir+"/"+string(dirp->d_name);
                files.push_back(filename);
                file_sizes[filename] = filesize(filename.c_str());
            }
        }
        else
            if(string(dirp->d_name) != "." && string(dirp->d_name) != "..")
            {
                string filename = dir+"/"+string(dirp->d_name);
                files.push_back(filename);
                file_sizes[filename] = filesize(filename.c_str());
            }
    }
    closedir(dp);
    return 0;
}

/**
 * @brief Collects all files, staring with some dir
 * @param [in] ext Extention of file that need to collect
 * @param [in] start_dir Entry dir path
 * @param [in/out] files Files, which contains in dir
 * @return True If everithing is ok
 */
bool IndexBuilder::get_dirs(const string ext, const string start_dir, vector<string> &files)
{
    queue<string> dirs = queue<string>();
    string next_dir;

    getdir(ext, start_dir, files, dirs);
    while(dirs.size() != 0)
    {
        next_dir = dirs.front();
        dirs.pop();
        getdir(ext, next_dir, files, dirs);
    }
    return true;
}

 
template<typename T>
vector<T> slice(vector<T> const &v, int s, int e)
{
    auto first = v.cbegin() + s;
    auto last = v.cbegin() + e + 1;

    vector<T> vec(first, last);
    return vec;

}


doc_list IndexBuilder::form_the_doclist()
{
    doc_list result;
    int counter = 0;
    while(counter < 500000000 || collection.size() == 0)
    {
        string file = collection.back();
        collection.pop_back();
        result.push_back(file);
        counter = counter + file_sizes[file];
    }
    return result;
}

void IndexBuilder::BSBITest()
{
    InvertIndex index;
    doc_list result;
    doc_list tmp = collection;
    BinarySaverData saver("./temp/");
    while(tmp.size() != 0)
    {
        // get some name for next directory
        while( get_free_memory() > (500 * 1024) || tmp.size() == 0)
        {
            string file = tmp.back();
            tmp.pop_back();
            index.indexing_file(file);
        }
        //index.save()
    
    }
} 
/*
The core of large-scale indexing algorithm

    IndexBuffer b1("store/instance1/index.bin");
    IndexBuffer b2("store/instance2/index.bin");
    vector<IndexBuffer> indicies = {b1, b2};
    inverted_list result;
    string min;
    word_position_map temp;
    while(indicies.size() > 1)
    {
        //find upper alphabetic word
        min = indicies[0].get_top_word();
        for(int i=1; i < indicies.size(); i++)
        {
            if(min.compare(indicies[i].get_top_word()) > 0) 
            {
                min = indicies[i].get_top_word();
            }
        }
        temp.clear();
        //merge and push to the result
        for(int i=0; i < indicies.size(); i++)
        {
            if(indicies[i].get_top_word().compare(min) == 0)
            {
                if(temp.size() == 0)
                {
                    temp = indicies[i].get_top_position_map();
                    if(indicies[i].next())
                    {
                        indicies.erase(indicies.begin() + i);
                    }
                }
                else
                {
                    temp = position_map_merge(temp, indicies[i].get_top_position_map());
                    if(indicies[i].next())
                    {
                        indicies.erase(indicies.begin() + i);
                    }
                }
            }
        }
        result[min] = temp;
    }
    //merge remain map
    for(auto i:indicies[0].GetInvertedIndex())
    {
        result[i.first] = i.second;
    } */