# SearchEngine

A simple search engine on C++, that will be a core of the future project.

Now it supports creating an inverted index and interface for it, including BM25 ranking. There is a SWIG wrap for the python as well.

Basic usage:

```cpp

#include "InvertIndex.h" 

int main()
{
    InvertIndex inv("/home/your_username', "txt");
    inv.build_index();
    for(auto& i:  inv.find(vector<string> {"int", "#include" }))
        cout<<i.first<<" "<<i.second<<endl;
}
```

