#include "SaverData.h"


TextFileSaverData::TextFileSaverData(string path)
{
    path_ = path;
}


void TextFileSaverData::save(InvertIndex *instance)
{
    string table = "table.txt", table_len = "table_len.txt";
    
    ofstream myfile;
    myfile.open (path_ + table);
    //myfile << "Writing this to a file.\n";
    for(auto i: (*instance).GetN2D())
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
    //return 0;
}