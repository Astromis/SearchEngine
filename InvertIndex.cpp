#include "InvertIndex.h"
#include <algorithm>
#include <utility>
#include <set>

mutex mut;


//TODO: 
// saving data
// make tokenization (boost?)
// what does program should do if amount of files would be much more then memory?
//      The one idea is block writing in file by numerical condition.
// Speed.Probably, it would be a good idea to create several instances of inverted index.
//      Each index is owned by separated threads.
//  By the way, writting in file could done threadable. Just use seek() to get a posobility writting data in one file by several threads.

InvertIndex::InvertIndex()
{
}

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
        int word_count = 0;
        while (in >> word)
        {
            //add conditions, that cut all
            mut.lock();
            num2doc[i] = f[i].c_str();
            doc2num[f[i].c_str()] = i;
            index[word][i].push_back((int)(in.tellg()) - (word.length()));
            mut.unlock();
            word_count++;
        }
        doc_length[i] = word_count;
        average_doc_length += word_count;
        word_count = 0;
    }
    average_doc_length /= document_count;

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
        //This condition is for the exact file extension. If it is not present, all file names to vector will be appended.
        if(ext != "")
        {
            if(string(dirp->d_name).find(ext+"\0") != -1)
                files.push_back(dir+"/"+string(dirp->d_name));
        }
        else
            if(string(dirp->d_name) != "." && string(dirp->d_name) != "..")
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

/* bool InvertIndex::intersect(vector<int> &result, string q1, string q2)
{
    // return false if no result for one of the quary
    word_position_map p1 = index[q1];
    word_position_map p2 = index[q2];
    if(p1.empty() || p2.empty()) return false;

    //sorting by increasing key numbers
    sort(p1.begin(),p1.end(),[this](int first, int second){ return first < second; } );
    sort(p2.begin(),p2.end(),[this](int first, int second){ return first < second; } );

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
    return true;
} */

vector<int> InvertIndex::intersect(vector<int> past, string q2)
{
    // get keys from map and put in vector
    auto func = [](word_position_map en)
    {
        vector<int> r;
        for(auto i: en)
            r.push_back(i.first);
        sort(r.begin(), r.end());
        return r;
    };

    vector<int> result;
    // return false if no result for one of the quary
    vector<int> p2 = func(index[q2]);
    // if(p2.empty()) return vector<string>;

    vector<int>::iterator it1 = past.begin();
    vector<int>::iterator it2 = p2.begin();

    do
    {
        if((*it1) == (*it2))
        {
            result.push_back((*it1));
            it1++;
            it2++;
        }
         else if((*it1) < (*it2))
                it1++;
        else it2++;
    }
    while(it1 != past.end() && it2 != p2.end());

    return result;
}

// get keys from map and put in vector
auto func = [](word_position_map en)
{
    vector<int> r;
    for(auto i: en)
        r.push_back(i.first);
    sort(r.begin(), r.end());
    return r;
};


vector<int> InvertIndex::MultipleIntersect(vector<string> quary)
{

    //sort vector by increasing frequency
    auto comp = [this](string first, string second)
    {
        return get_tf(first) > get_tf(second);
    };



    sort(quary.begin(), quary.end(), comp);
    // word_position_map temp_map;
    // vector<string> cycle_temp_map;

    vector<int> result = func(index[quary.back()]);
    quary.pop_back();
    
    for(auto& i: quary)
    {        
        result = intersect(result, i);
    }
    
    return result;
} 

vector<string> InvertIndex::operator[](string q)
{
    vector<string> resp;
    if(index.find(q) == index.end()) return vector<string>();
    for(auto& it: index[q])
        resp.push_back(num2doc[it.first]);
    return resp;
}


size_t InvertIndex::get_tfd(string  word_instance, int doc_instance)
{
    size_t tf = index[word_instance][doc_instance].size();
    
    return tf;
}

float InvertIndex::get_idf(string word_instance)
{
    int size = index[word_instance].size();
    return log(document_count/size);

}

float InvertIndex::get_smoothed_idf(string word_instance)
{
    int size = index[word_instance].size();
    cout<<"Size: "<<size<<endl;
    return log( (document_count - size + 0.5) / (size + 0.5) );
}

float InvertIndex::get_tf_idf(string word,int document)
{
    return get_tfd(word, document) / get_idf(word);
}

float InvertIndex::get_tf(string word)
{
    // It is not a good idea. 
    // Mannig said that this number should be precomputed and is being kept in the memory.
    word_position_map temp = index[word];
    float freq = 0;
    for(auto& inst: temp)
        freq += inst.second.size();
    return freq;
}

float InvertIndex::BM25(string word, int document)
{
    float k = 2;
    float b = 0.75;
    float numerator = get_tfd(word, document) * (k + 1) * get_idf(word);
    float denumenator =  get_tfd(word, document) + k * (1 - b  + b * doc_length[document] / average_doc_length);
    return numerator / denumenator;
}

 float InvertIndex::BM25(vector<string> word, int document)
{
    float tmp = 0;
    for(string quary: word)
    {
        tmp += BM25(quary, document);
    }
    return tmp;
} 

vector< pair<float, string> > InvertIndex::find(string quary)
{
    word_position_map docs = index[quary];
    vector< pair<float, string> > result;
    pair<float, string> temp;

    // perform ranking
    for(auto doc: docs)
        // getting of idf for each word should be obtained here because of high computational cost
        {
            result.push_back(make_pair(BM25(quary, doc.first), num2doc[doc.first]));
        }

    sort(result.begin(), result.end());
    return result;
    /* for(vector< pair<float, string> >::iterator it=result.begin(); it != result.end(); it++)
        cout<<it->second<<" "<<it->first<<endl; */
        }

vector< pair<float, string> > InvertIndex::find(vector<string> quary)
{
    vector<int>  docs = MultipleIntersect(quary);
    vector< pair<float, string> > result;
    pair<float, string> temp;

    // perform ranking
    for(auto doc: docs)
        // getting of idf for each word should be obtained here because of high computational cost
        {
            result.push_back(make_pair(BM25(quary, doc), num2doc[doc]));
        }

     sort(result.begin(), result.end());
     return result;
    /* for(vector< pair<float, string> >::iterator it=result.begin(); it != result.end(); it++)
        cout<<it->second<<" "<<it->first<<endl; */
}

void InvertIndex::save(SaverData& saver)
{
    saver.save(this);
}

void TextFileSaverData::save(InvertIndex *instance)
{
    write_simple_map(instance->GetD2L(), table_len);

}

template<class mapType>
void TextFileSaverData::write_simple_map(mapType map, string file_name)
{
    ofstream myfile;
    myfile.open (path_ + file_name, ios::out);
    for(auto i: map)
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
}