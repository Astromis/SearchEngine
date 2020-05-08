#include "InvertedIndex.h"

class IndexInterface
{
    public:
        IndexInterface();
        IndexInterface(InvertedIndex& d);
        virtual ~IndexInterface();


        //void threadIndexing(vector<string> &files, inverted_list &index);
        vector<int> intersect(vector<int> past, string q2);
        int MultipleIntersect(vector<string> quary, vector<int>& result);
        //interface
        vector<int>  find(vector<string> quary_words);
        
        float get_tf(string word);

        vector<string> operator[](string q);
    protected:


    private:
    InvertedIndex idx;
};