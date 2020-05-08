#include "InvertedIndex.h"
#include <algorithm>

class IRanker
{
    protected:
    InvertedIndex idx;
    public:
    virtual vector< pair<float, int> > rank(vector<int>& doc_id, vector<string> quary_words) = 0;
};


class BM25Ranker: public IRanker
{
    
    public:
    BM25Ranker();
    BM25Ranker(InvertedIndex i);
    ~BM25Ranker();
    float get_tfd(string  word_instance, int doc_instance);
    float get_tf(string word);
    float get_idf(string word_instance);
    float get_tf_idf(string word,int document);
    float get_smoothed_idf(string word_instance);
    float BM25(vector<string> word, int document);
    float BM25_kernel(string word, int document);
    vector< pair<float, int> > rank(vector<int>& doc_ids, vector<string> quary_words) override;

};