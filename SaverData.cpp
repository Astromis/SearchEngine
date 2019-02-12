#include "SaverData.h"


/* void TextFileSaverData::save(InvertIndex *instance)
{
    string table = "table.txt", table_len = "table_len.txt";
    
    ofstream myfile;
    myfile.open (path_ + table);
    //myfile << "Writing this to a file.\n";
    for(auto i: (*instance).GetN2D())
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
    //return 0;
} */


TextFileSaverData::TextFileSaverData(string path)
{
    path_ = path; 
}

void TextFileSaverData::save(InvertIndex *instance)
{
    write_simple_map(instance->GetD2L(), table_len);

}

template<class mapType>
void TextFileSaverData::write_simple_map(mapType map, string file_name)
{
    ofstream myfile;
    myfile.open (path_ + file_name, ios::out);
    for(auto i: map)
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
}