#include "Ranker.hpp"


BM25Ranker::BM25Ranker(InvertedIndex i)
{
    idx = i;
}
BM25Ranker::BM25Ranker()
{
    
}

BM25Ranker::~BM25Ranker()
{

}

/**
 * @brief Calculates frequency of word in one document
 */
float BM25Ranker::get_tfd(string word_instance, int doc_id)
{
    float tf = idx.index[word_instance][doc_id].size();
    return tf;
}

/**
 * @brief Calculates IDF of word
 */
float BM25Ranker::get_idf(string word_instance)
{
    float size = idx.index[word_instance].size();
    return log(idx.document_count/size);
}

/**
 * @brief Calculates smoothed IDF of word
 */
float BM25Ranker::get_smoothed_idf(string word_instance)
{
    float size = idx.index[word_instance].size();
    return log( (idx.document_count - size + 0.5) / (size + 0.5) );
}

/**
 * @brief Calculates TF-IDF of word for one document
 */
float BM25Ranker::get_tf_idf(string word, int doc_id)
{
    return get_tfd(word, doc_id) / get_idf(word);
}

/**
 * @brief Get a frequency of word through all documents
 */
float BM25Ranker::get_tf(string word)
{
    // It is not a good idea. 
    // Mannig said that this number should be precomputed and is being kept in the memory.
    word_position_map temp = idx.index[word];
    float freq = 0;
    for(auto& inst: temp)
        freq += inst.second.size();
    return freq;
}

/**
 * @brief Computes BM25 kernel multiplier or actual result for a one-word quary
 */
float BM25Ranker::BM25_kernel(string word, int doc_id)
{
    float k = 2;
    float b = 0.75;
    float numerator = get_tfd(word, doc_id) * (k + 1);
    numerator = numerator * get_smoothed_idf(word);
    float denominator =  get_tfd(word, doc_id) + k * (1 - b  + b * idx.doc_length[doc_id] / idx.average_doc_length);
    return get_smoothed_idf(word) * numerator / denominator;
}
/**
 * @brief Computes BM25 ranking function for quary
 * @param  word Vector of quares
 * @param  doc Document
 * @return BM25 result for a quary
 */
 float BM25Ranker::BM25(vector<string> word, int doc_id)
{
    float tmp = 0;
    for(string quary: word)
    {
        tmp += BM25_kernel(quary, doc_id);
    }
    return tmp;
} 

/**
 * @brief Perform the ranking of result
 * @param doc_ids a reference to document IDs
 * @param quary_words vector of tokens to search
 */
vector< pair<float, int> > BM25Ranker::rank(vector<int>& doc_ids, vector<string> quary_words)
{
    vector< pair<float, int> > result;
    pair<float, string> temp;

    // perform ranking
    for(auto doc_id: doc_ids)
        // getting of idf for each word should be obtained here because of high computational cost
        {
            result.push_back(make_pair(BM25(quary_words, doc_id), doc_id));
        }

    sort(result.begin(), result.end());
    return result;
}