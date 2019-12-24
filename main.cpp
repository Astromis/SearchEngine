#include "IndexBuilder.hpp"
#include "InvertIndex.h"
#include "SaverData.h"

#include "utils.hpp"

#include <map>
#include <ctime>


int main(int argc, char **argv)
{

    if(argc != 2)
    {
        cout<<"Input parameters is not enough."<<endl;
        return 0;
    }
    InvertIndex inv;
    IndexBuilder builder_t(string(argv[1]), "", 1);
    clock_t begin = clock();
    builder_t.build_index_from_collection(&inv);
    clock_t end = clock();
    cout<<"Ellapsed time: "<<double(end - begin) / CLOCKS_PER_SEC<<endl;
    for(auto& i:  inv.find(vector<string> {"int", "#include"  }))
        cout<<i.first<<" "<<i.second<<endl;
    //inv.clear_index();

    //builder_t.index_collection(&inv);
    //for(auto& i:  inv.find(vector<string> {"int", "#include" }))
    //    cout<<i.first<<" "<<i.second<<endl; 

    /* vector<string> res;
   
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl;  */
    
    string root = "store/";
    string inst = "instance/";
    BinarySaverData saver(root);
    cout<<"Saving..."<<endl;
    inv.save(saver, inst);
    cout<<"***********************************"<<endl;
    InvertIndex loaded_in;
    cout<<"Loading..."<<endl;
    loaded_in.load(saver, inst);
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl;

    return 0;
}
