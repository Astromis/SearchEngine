#include "IndexBuilder.hpp"
#include "InvertIndex.h"
#include "SaverData.h"

#include "utils.hpp"

#include <map>
#include <ctime>
#include <algorithm>




int main(int argc, char **argv)
{

    /* if(argc != 2)
    {
        cout<<"Input parameters is not enough."<<endl;
        return 0;
    } */


    //InvertIndex inv1;
    IndexBuilder builder_t("", "", 1);
    builder_t.BSBITest();
    //first chunk
    //builder_t.set_start_path("test_fol2/");
    //clock_t begin = clock();
    //builder_t.build_index_from_collection(&inv1);
    //clock_t end = clock();
    /* vector<IndexBuffer> indicies(2);
    for(int i=0; i < 2;  i++)
    {
        //cout<<temp_storages[ + "index.bin"<<endl;
        string path = "store/store/instance" + to_string(i) + "/index.bin";
        //IndexBuffer ib(path, 5);
        indicies[i] = IndexBuffer(path);
        
    } */

    /* InvertIndex inv1, inv2;
    string root = "store/";
    string inst1 = "instance1/";
    string inst2 = "instance2/";
    IndexBuilder builder_t("", "", 1);
    BinarySaverData saver(root); */

    /* //first chunk
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
    inv2.save(saver, inst2); */
   
    /* IndexBuffer b2("store/store/instance1/index.bin", 100);
    IndexBuffer b3("store/store/instance2/index.bin", 100);
    IndexBuffer b1("store/store/instance0/index.bin", 100);
    vector<IndexBuffer> indicies = {b1, b2, b3}; */
    /* inverted_list result;
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

    for(auto i: result)
        cout<<i.first<<endl;  */
    /* for(auto i: inv1.GetIndex())
        cout<<i.first<<endl; */

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
