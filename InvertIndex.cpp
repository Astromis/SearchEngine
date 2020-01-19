#include "InvertIndex.h"
#include "SaverData.h"
#include <algorithm>
#include <utility>
#include <set>

mutex mut;
int InvertIndex::doc_id = 0;

//TODO: 
// make tokenization (boost?)
// what does program should do if amount of files would be much more then memory?
//      The one idea is block writing in file by numerical condition.
// Speed.Probably, it would be a good idea to create several instances of inverted index.
//      Each index is owned by separated threads.
//  By the way, writting in file could done threadable. Just use seek() to get a posobility writting data in one file by several threads.

InvertIndex::InvertIndex()
{
   // doc_id = 0;
}

InvertIndex::InvertIndex(string path, string ext)
{
    /* pathfolder = path;  
    extention = ext; */
    doc_id = 0;
    //ctor
}

InvertIndex::~InvertIndex()
{
    //dtor
}


/**
 * @brief index only one file
 * @param file file need to be indexing
 * @return true if successfully
 */
bool InvertIndex::indexing_file(string file)
{
    average_doc_length *= document_count;
    ifstream in;
    in.open(file.c_str(), ifstream::in);
    string word;
    int word_count = 0;
    while (in >> word)
    {
        //add conditions, that cut all
        num2doc[doc_id] = file.c_str();
        doc2num[file.c_str()] = doc_id;
        index[word][doc_id].push_back((int)(in.tellg()) - (word.length()));
        word_count++;
    }
    doc_length[doc_id] = word_count;
    average_doc_length += word_count;
    word_count = 0;
    document_count++;
    doc_id++;
    average_doc_length /= document_count;

}


/**
 * @brief Index the files from the vector
 * @param files the vector of the file paths
 * @return True if index has been built successfully
 */

bool InvertIndex::indexing_collection(doc_list& files)
{  
    
    for (unsigned int i = 0;i < files.size();i++)
    {
        indexing_file(files[i]);
    }

    cout<<"Indexing complete in thread id "<<this_thread::get_id()<<". Size: "<< index.size()<<endl;   
    return true;
}



/**
 * @brief Performs intersection between vectors of docID
 * @param [in] past Actual vector of docID
 * @param [in] q2 String by which the next vector to be intersect is obtained
 * @return Resulted vector
 */
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

/**
 * @brief Performs intersection with multiple quaries
 * @param [in] quary Vector of quaries(strings, means words) that need to be intersected
 * @return result Resulted vector of docID
 */
vector<int> InvertIndex::MultipleIntersect(vector<string> quary)
{

    //sort vector by increasing frequency
    auto comp = [this](string first, string second)
    {
        return get_tf(first) > get_tf(second);
    };

    sort(quary.begin(), quary.end(), comp);

    vector<int> result = func(index[quary.back()]);
    quary.pop_back();
    
    for(auto& i: quary)
    {        
        result = intersect(result, i);
    }
    
    return result;
} 

/**
 * @breif Overload [] operator by getting a word_position_map
 * @param [in] q Quary string
 * @return resp Word position map 
 */
vector<string> InvertIndex::operator[](string q)
{
    vector<string> resp;
    if(index.find(q) == index.end()) return vector<string>();
    for(auto& it: index[q])
        resp.push_back(num2doc[it.first]);
    return resp;
}

/**
 * @brief Calculates frequency of word in one document
 */
size_t InvertIndex::get_tfd(string word_instance, int doc_instance)
{
    size_t tf = index[word_instance][doc_instance].size();
    return tf;
}

/**
 * @brief Calculates IDF of word
 */
float InvertIndex::get_idf(string word_instance)
{
    float size = index[word_instance].size();
    return log(document_count/size);

}

/**
 * @brief Calculates smoothed IDF of word
 */
float InvertIndex::get_smoothed_idf(string word_instance)
{
    float size = index[word_instance].size();
    return log( (document_count - size + 0.5) / (size + 0.5) );
}

/**
 * @brief Calculates TF-IDF of word for one document
 */
float InvertIndex::get_tf_idf(string word, int document)
{
    return get_tfd(word, document) / get_idf(word);
}

/**
 * @bried Get a frequency of word through all documents
 */
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

/**
 * @brief Computes BM25 kernel multiplier or actual result for a one-word quary
 */
float InvertIndex::BM25_kernel(string word, int document)
{
    float k = 2;
    float b = 0.75;
    float numerator = get_tfd(word, document) * (k + 1);
    numerator = numerator * get_smoothed_idf(word);
    float denominator =  get_tfd(word, document) + k * (1 - b  + b * doc_length[document] / average_doc_length);
    return get_smoothed_idf(word) * numerator / denominator;
}
/**
 * @brief Computes BM25 ranking function for quary
 * @param [in] word Vector of quares
 * @param [in] doc Document
 * @retrun BM25 result for a quary
 */
 float InvertIndex::BM25(vector<string> word, int document)
{
    float tmp = 0;
    for(string quary: word)
    {
        tmp += BM25_kernel(quary, document);
    }
    return tmp;
} 

/**
 * @brief Performs a search of document, rinking they by BM25 (one word)
 * @param quary Word to be searched
 * @return Vector of pairs of doc and their ranking value in descending order
 */ 
vector< pair<float, string> > InvertIndex::find(string quary)
{
    word_position_map docs = index[quary];
    vector< pair<float, string> > result;
    pair<float, string> temp;

    // perform ranking
    for(auto doc: docs)
        // getting of idf for each word should be obtained here because of high computational cost
        {
            result.push_back(make_pair(BM25_kernel(quary, doc.first), num2doc[doc.first]));
        }

    sort(result.begin(), result.end());
    return result;
}

/**
 * @brief Performs a search of document, rinking they by BM25
 * @param [in] quary Vector of words to be searched
 * @return Vector of pairs of doc and their ranking value in descending order
 */ 
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
}

/**
 * @brief Saves the inverted index
 * @param saver Object of SaverData famaly classess,
 */ 
void InvertIndex::save(SaverData& saver, string dir_instance)
{
    saver.save(this, dir_instance);
}

/**
 * @brief Loads the inverted index
 * @param saver Object of SaverData famaly classess,
 */ 
void InvertIndex::load(SaverData& saver, string dir_instance)
{
    saver.load(this, dir_instance);
}

void InvertIndex::clear_index()
{
    index.clear();
}