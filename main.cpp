#include "IndexBuilder.hpp"
#include "InvertIndex.h"
#include "SaverData.h"

#include "utils.hpp"

#include <map>
#include <ctime>
#include <algorithm>


word_position_map position_map_merge(word_position_map m1, word_position_map m2)
{
    for(auto i:m1)
        m2[i.first] = i.second;
    return m2;
}


int main(int argc, char **argv)
{

    /* if(argc != 2)
    {
        cout<<"Input parameters is not enough."<<endl;
        return 0;
    } */


    InvertIndex inv1, inv2;
    string root = "store/";
    string inst1 = "instance1/";
    string inst2 = "instance2/";
    IndexBuilder builder_t("", "", 1);
    BinarySaverData saver(root);

    //first chunk
    builder_t.set_start_path("test_fol/");
    //clock_t begin = clock();
    builder_t.build_index_from_collection(&inv1);
    //clock_t end = clock();

    cout<<"Saving first chunk..."<<endl;
    inv1.save(saver, inst1);
    //second chunk
    builder_t.set_start_path("test_fol1/");
    //clock_t begin = clock();
    builder_t.build_index_from_collection(&inv2);
    //clock_t end = clock();
    cout<<"Saving second chunk..."<<endl;
    inv2.save(saver, inst2);
    
    IndexBuffer b1("store/instance1/index.bin");
    IndexBuffer b2("store/instance2/index.bin");
    vector<IndexBuffer> indicies = {b1, b2};
    inverted_list result;
    string min;
    word_position_map temp;
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
    /* 
    vector< inverted_list::iterator> indices = {inv1.GetIndex().begin(), inv2.GetIndex().begin()};
    inverted_list result;
    vector<string> top_words;
    word_position_map temp;
    
    while(indices[0] != inv1.GetIndex().end() && indices[1] != inv2.GetIndex().end())
    {
        top_words.clear();
        for(int i = 0; i < indices.size(); i++)
        {
            top_words.push_back(indices[i]->first);
        }
        
        temp.clear();
        for(int i = 0; i < top_words.size(); i++)
        {
            temp = indices[i]->second;
            for(int j = 1; j < top_words.size(); j++)
            {
                if(top_words[i] != top_words[j])
                {
                    continue;
                }
                else
                {
                    temp = position_map_merge(temp, indices[j]->second);
                }
            }
            result[top_words[i]] = temp;
        }
        indices[0]++;
        indices[1]++;
    } */

    for(auto i: result)
        cout<<i.first<<endl;

/*     for(auto i: inv1.GetIndex())
        cout<<i.first<<endl;

    for(auto i: inv2.GetIndex())
        cout<<i.first<<endl; */

    /* vector<string> res;
   
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl;  */

/*
    cout<<"Ellapsed time: "<<double(end - begin) / CLOCKS_PER_SEC<<endl;
    for(auto& i:  inv.find(vector<string> {"int", "#include"  }))
        cout<<i.first<<" "<<i.second<<endl;
    //inv.clear_index();

    //builder_t.index_collection(&inv);
    //for(auto& i:  inv.find(vector<string> {"int", "#include" }))
    //    cout<<i.first<<" "<<i.second<<endl; 

//here
    
    string root = "store/";
    string inst = "instance1/";
    BinarySaverData saver(root);
    cout<<"Saving..."<<endl;
    inv.save(saver, inst);
    cout<<"***********************************"<<endl;
    InvertIndex loaded_in;
    cout<<"Loading..."<<endl;
    loaded_in.load(saver, inst);
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl;
*/
    return 0;
}
