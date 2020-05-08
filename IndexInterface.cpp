#include "IndexInterface.hpp"
#include <algorithm>


IndexInterface::IndexInterface()
{
   // doc_id = 0;

}

IndexInterface::IndexInterface(InvertedIndex& d)
{
    idx = d;

}

IndexInterface::~IndexInterface()
{
    //dtor
}

/**
 * @brief Performs intersection between vectors of docID
 * @param [in] past Actual vector of docID
 * @param [in] q2 String by which the next vector to be intersect is obtained
 * @return Resulted vector
 */
vector<int> IndexInterface::intersect(vector<int> past, string q2)
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
    vector<int> p2 = func(idx.index[q2]);
    if(p2.size() == 0)
    {
        return p2;
    }

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
int IndexInterface::MultipleIntersect(vector<string> quary, vector<int>& result)
{

    //sort vector by increasing frequency
    auto comp = [this](string first, string second)
    {
        return get_tf(first) > get_tf(second);
    };

    sort(quary.begin(), quary.end(), comp);

    while(result.size() == 0)
    {
        result = func(idx.index[quary.back()]);
        quary.pop_back();
        if(quary.size() == 0)
        {
            return 1;
        }
    }
    
    for(auto& i: quary)
    {        
        result = intersect(result, i);
    }
    
    return 0;
} 

/**
 * @breif Overload [] operator by getting a word_position_map
 * @param [in] q Quary string
 * @return resp Word position map 
 */
vector<string> IndexInterface::operator[](string q)
{
    vector<string> resp;
    if(idx.index.find(q) == idx.index.end()) return vector<string>();
    for(auto& it: idx.index[q])
        resp.push_back(idx.num2doc[it.first]);
    return resp;
}

/**
 * @bried Get a frequency of word through all documents
 */
float IndexInterface::get_tf(string word)
{
    // It is not a good idea. 
    // Mannig said that this number should be precomputed and is being kept in the memory.
    word_position_map temp = idx.index[word];
    float freq = 0;
    for(auto& inst: temp)
        freq += inst.second.size();
    return freq;
}


vector<int>  IndexInterface::find(vector<string> quary_words)
{
    vector<int>  doc_ids;
    MultipleIntersect(quary_words, doc_ids);
    return doc_ids;
}