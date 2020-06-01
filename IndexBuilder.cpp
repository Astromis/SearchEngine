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

bool IndexBuilder::is_collection_empty()
{
    return collection.size() > 0 ? false : true;
}

/**
 * @brief Generate names for saving
 * @param base base name
 * @return generated name with slash 
 */ 
string generate_name(string base)
{
    static string aux = "instance";
    static int count = -1;
    count++;
    return base + aux + to_string(count) + "/";
}

word_position_map position_map_merge(word_position_map m1, word_position_map m2)
{
    for(auto i:m1)
        m2[i.first] = i.second;
    return m2;
}


/**
 * @brief dummy(tokenization by spaces) indexing of only one file
 * @param file file need to be indexing
 * @param index the reference of inverted index structure wehere the data will stored in
 * @return true if successfully
 */
bool IndexBuilder::indexing_file(string file, InvertedIndex& index)
{
    index.average_doc_length *= index.document_count;
    ifstream in;
    in.open(file.c_str(), ifstream::in);
    string word;
    int word_count = 0;
    while (in >> word)
    {
        //add conditions, that cut all
        index.num2doc[index.doc_id] = file.c_str();
        index.doc2num[file.c_str()] = index.doc_id;
        index.index[word][index.doc_id].push_back((int)(in.tellg()) - (word.length()));
        
        //cout << word <<"\r" << flush;
        
        word_count++;
    }
    
    //std::cout << endl;
    
    index.doc_length[index.doc_id] = word_count;
    index.average_doc_length += word_count;
    word_count = 0;
    index.document_count++;
    index.doc_id++;
    index.average_doc_length /= index.document_count;

}


/**
 * @brief Index the files from the collection of documents
 * @param files the reference of a vector of the file paths
 * @param index the reference of inverted index structure wehere the data will stored in
 * @return True if index has been built successfully, return false if memory limit was exceeded
 */

bool IndexBuilder::_indexing_collection(doc_list& files, InvertedIndex& index)
{  
    
    /* for (unsigned int i = 0;i < files.size();i++)
    {
        indexing_file(files[i]);
    } */
    int starting_point = get_free_memory();
    while (files.size() > 0)
    {
        //FIXME: remove constant value of a memory treshold
        if((starting_point - get_free_memory()) > 200000)
        {
            cout<<get_free_memory()<<endl;
            cout<<"The free memory has exceeded the threshold"<<endl;
            return false;
        }
        indexing_file(files[files.size() - 1], index);
        files.pop_back();   
    }
    

    cout<<"Indexing complete in thread id "<<this_thread::get_id()<<". Size: "<< index.index.size()<<endl;   
    return true;
}

bool IndexBuilder::indexing_collection(InvertedIndex& index)
{  
    return _indexing_collection(collection, index);
}

/**
 * @brief Function for large scale indexing with storing intermidiate results on the hard drive(under development)
 */
void IndexBuilder::BSBITest()
{
    vector<InvertedIndex> inv(1);
    //IndexBuilder builder_t("", "", 1);
    set_start_path("test_fol/");
    string root = "store/";
    BinarySaverData saver(root);
    string temp_name;
    vector<string> temp_storages;
    int saving_counter = 1;
    while (!is_collection_empty())
    {
        if(!_indexing_collection(collection, inv[inv.size()-1])){
            temp_name = generate_name(root);
            temp_storages.push_back(temp_name);
            inv[inv.size()-1].save(saver, temp_name);
            inv[inv.size()-1].clear_index();
            inv.push_back(InvertedIndex());
            saving_counter++;
        }
        //else exit(1);
    }
    temp_name = generate_name(root);
    temp_storages.push_back(temp_name);
    inv[inv.size()-1].save(saver, temp_name);
    cout<<"Indexing complited"<<endl;
    vector<IndexBuffer> indicies(saving_counter);
    for(int i=0; i < saving_counter; i++)
    {
        cout<<temp_storages[i] + "index.bin"<<endl;
        IndexBuffer a("store/" + temp_storages[i] + "index.bin");
        indicies[i] = a;
    }
    //inverted_list result;
    IndexBufferO result("result.bin");
    string min;
    word_position_map temp;
    cout<<"Merging all indicies into one"<<endl;
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
    }
    cout<<"Merging complited"<<endl;
    //exit(1);
} 
