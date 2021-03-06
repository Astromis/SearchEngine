#include "InvertIndex.h"
#include "SaverData.h"
#include <map>
#include <ctime>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cout<<"Input parameters is not enough."<<endl;
        return 0;
    }
    InvertIndex inv(string(argv[1]), "");
    clock_t begin = clock();
    inv.build_index();
    clock_t end = clock();
    cout<<"Ellapsed time: "<<double(end - begin) / CLOCKS_PER_SEC<<endl;
    vector<string> res;
   
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl; 
    
    string path_a = "./";
    BinarySaverData saver(path_a);
    cout<<"Saving..."<<endl;
    inv.save(saver);
    cout<<"***********************************"<<endl;
    InvertIndex loaded_in;
    cout<<"Loading..."<<endl;
    loaded_in.load(saver);
    for(auto& i:  loaded_in.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl;

    return 0;
}
