#include "IndexBuilder.hpp"
#include "InvertIndex.h"
#include "SaverData.h"

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

    
    InvertIndex inv;
    IndexBuilder builder_t("/home/astromis/output_", "txt", 1);
    builder_t.build_index_from_collection(&inv);
    string root = "store/";
    string inst = "instance1/";
    BinarySaverData saver(root);
    //cout<<"Saving..."<<endl;
    //inv.save(saver, inst); 
    //InvertIndex loaded_in;
    //cout<<"Loading..."<<endl;
    //loaded_in.load(saver, inst);
    string input = "частота генератор";
    //cin>>input;
    while(1)
    {
    getline(cin, input);
    vector<string> quary;
    split(input, quary, ' ');
       
    for(auto& i:  inv.find(quary))
        cout<<i.first<<" "<<i.second<<endl;  

    }
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
