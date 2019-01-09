#include "InvertIndex.h"

mutex mut;


//TODO: 
// Correct multiintersect
// develop BM25
// reconstruct data structure in oreder to have possibility of sorting with in-built methods
// saving data
// make tokenization (boost?)

InvertIndex::InvertIndex(string path, string ext)
{
    pathfolder = path;
    extention = ext;
    //ctor
}

InvertIndex::~InvertIndex()
{
    //dtor
}

bool InvertIndex::build_index()
{
    doc_list f;
    get_dirs(extention ,pathfolder, f);
    document_count = f.size();
    for (unsigned int i = 0;i < document_count;i++)
    {
        ifstream in;
        in.open(f[i].c_str(), ifstream::in);
        string word;

        while (in >> word)
        {
            //add conditions, that cut all
            mut.lock();
            index[word][f[i].c_str()].push_back((int)(in.tellg()) - (word.length()));
            mut.unlock();
        }
    }
    cout<<"Indexing complete in thread id "<<this_thread::get_id()<<". Size: "<< index.size()<<endl;   
    return true;
}

void InvertIndex::threadIndexing(vector<string> &files, inverted_list &index)
{
    vector<string> a(files.begin(), files.begin() + files.size()/ 2);
    vector<string> b(files.begin() + files.size()/ 2, files.end());

/*     thread thread1(build_index, ref(a));
    thread thread2(build_index, ref(b));
    thread1.join();
    thread2.join(); */
}

int InvertIndex::getdir (const string ext, const string dir, vector<string> &files, queue<string> &dirs)
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
        //This condition for the exact file extension. If it is not present, all file names to vector will be append.
        if(ext != "")
        {
            if(string(dirp->d_name).find(ext+"\0") != -1)
                files.push_back(dir+"/"+string(dirp->d_name));
        }
        else
            files.push_back(dir+"/"+string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

bool InvertIndex::get_dirs(const string ext, const string start_dir, vector<string> &files)
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

bool InvertIndex::get_position_vector(position_vector *response, word_position_map &data, string &quary)
{
    word_position_map::iterator it = data.find(quary);
    if(it != data.end())
    {
        *response = it->second;
        return true;
    }
    else return false;
}

bool InvertIndex::get_word_position_map(word_position_map *response, string &quary)
{
    inverted_list::iterator it = this->index.find(quary);
    if(it != index.end())
    {
        *response = it->second;
        return true;
    }
    else return false;
}

bool InvertIndex::intersect(vector<string> &result, string q1, string q2)
{
    // return false if no result for one of the quary
    word_position_map p1 = index[q1];
    word_position_map p2 = index[q2];
    if(p1.empty() || p2.empty()) return false;
    word_position_map::iterator it1 = p1.begin();
    word_position_map::iterator it2 = p2.begin();
    while(it1 != p1.end() && it2 != p2.end())
    {
        if(it1->first == it2->first)
        {
            result.push_back(it1->first);
            it1++;
            it2++;
        }
        else if(it1->first < it2->first)
                it1++;
        else it2++;
    }
    //cout<< count;
    return true;
}
/* 
bool InvertIndex::MultipleIntersect(vector<string> *result, vector<string> quary)
{
    //sort vector by increasing frequency
    word_position_map temp_map;
    vector<string> cycle_temp_map;
    *result = index[quary.back()];
    quary.pop_back();
    word_position_map::iterator it = temp_map.begin();
    while(quary.size() != 0 && it != temp_map.end())
    {
        temp_map = index[quary.back()];
        InvertIndex::intersect(&cycle_temp_map, result, &temp_map)
        (*result).insert(cycle_temp_map.begin(), cycle_temp_map.end());
        quary.pop_back();
    }
} */

vector<string> InvertIndex::operator[](string q)
{
    vector<string> resp;
    if(index.find(q) == index.end()) return vector<string>();
    for(auto& it: index[q])
        resp.push_back(it.first);
    return resp;
}


size_t InvertIndex::get_tf(string  word_instance, string doc_instance)
{
    size_t tf = index[word_instance][doc_instance].size();
    
    return tf;
}

float InvertIndex::get_idf(string word_instance)
{
    int size = index[word_instance].size();
    return log(document_count/size);

}

float InvertIndex::get_tf_idf(string word,string document)
{
    return get_tf(word, document) / get_idf(word);
}

int BM25()
{

}

int InvertIndex::ranking(string quary)
{
    word_position_map docs = index[quary];
    for(auto doc: docs)
        cout<<"For document "<<doc.first<<" TF-IDF is "<<get_tf_idf(quary, doc.first)<<endl;
    return 0;
}