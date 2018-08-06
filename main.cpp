#include "InvertIndex.h"

int main()
{
    InvertIndex inv(string("/home/astromis/git_packets"), string(".txt"));
    inv.build_index();
    vector<string> res;
    inv.intersect(res, "and", "asdfffff");
    for(auto r: res)
        cout<<r<<endl;
/*     for(auto ans: inv["in"])
        cout<<ans<<endl; */
    return 0;
}