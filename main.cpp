#include "InvertIndex.h"
#include <map>
int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cout<<"Input parameters is not enough."<<endl;
        return 0;
    }
    InvertIndex inv(string(argv[1]), string(".h"));
    inv.build_index();
    vector<string> res;
    inv.ranking("#include");
    //inv.intersect(res, "int", "#include");
/*     cout<<"TF: "<< inv.get_tf("#include", "codec.h")<< endl;
    cout<<"IDF: "<< inv.get_idf("#include")<< endl;
    cout<<"TF-IDF: "<< inv.get_tf_idf("#include", "codec.h")<< endl;
    cout<<"TF: "<< inv.get_tf("#include", "codec.h")<< endl;
    cout<<"IDF: "<< inv.get_idf("#include")<< endl;
    cout<<"TF-IDF: "<< inv.get_tf_idf("#include", "codec.h")<< endl; */

/*
    for(auto r: res)
        cout<<r<<endl;
     for(auto ans: inv["in"])
        cout<<ans<<endl; */
    return 0;
}
