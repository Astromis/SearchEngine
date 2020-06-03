#include "IndexBuilder.hpp"
#include "InvertedIndex.h"
#include "SaverData.h"
#include "Ranker.hpp"
#include "IndexInterface.hpp"
#include "contrib/Stemming-ru/source/StemmerPorter.h"

#include "utils.hpp"

#include <map>
#include <ctime>
#include <algorithm>


size_t split(const string &txt, vector<string> &strs, char ch)
// https://ideone.com/1RCZM8
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();
 
    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;
 
        pos = txt.find( ch, initialPos );
    }
 
    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );
 
    return strs.size();
}

int main(int argc, char **argv)
{

    /* if(argc != 2)
    {
        cout<<"Input parameters is not enough."<<endl;
        return 0;
    } */

    StemmerPorter sp;
    InvertedIndex inv;
    IndexBuilder builder_t("russian/", "", 1);
    builder_t.indexing_collection(inv);
   /*  string root = "store/";
    string inst = "instance1/";
    BinarySaverData saver(root);
    cout<<"Saving..."<<endl;
    inv.save(saver, inst); 
    InvertedIndex loaded_in;
    cout<<"Loading..."<<endl;
    loaded_in.load(saver, inst);*/
    string input = "#include int"; 
    //cin>>input;
    IndexInterface ifsace(inv);
    vector<int> founded_docs;

    BM25Ranker ranker(inv);

   /*  while(1)
    {
    getline(cin, input);
    vector<string> quary;
    split(input, quary, ' ');
    for(int i=0; i< quary.size(); i++)
    {
        quary[i] = sp.stemm(quary[i]);
        cout<<"LOG:"<<quary[i]<<endl;
    }
    founded_docs = ifsace.find(quary);
    for(auto& i:  ranker.rank(founded_docs, quary))
        cout<<i.first<<" "<<inv.num2doc[i.second]<<endl;  
    } */
//here
    

    /*
    cout<<"***********************************"<<endl;
    InvertIndex loaded_in;
    cout<<"Loading..."<<endl;
    loaded_in.load(saver, inst);
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl; */

    return 0;
}
