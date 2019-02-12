#ifndef SAVERDATA_H
#define SAVERDATA_H

#include "InvertIndex.h"
#include <iostream>
#include <fstream>

using namespace std;


class InvertIndex;

class SaverData
{
    public:

    virtual void save(InvertIndex *instance)=0;
    //virtual ~SaverData();
    string path_;
    protected:

};

class TextFileSaverData : public SaverData 
{
    
    public:
        TextFileSaverData(string path);
        void save(InvertIndex *instance)  override;
        template<class mapType>
        void write_simple_map(mapType map, string file_name);
        string path_;
        string table = "table.txt", table_len = "table_len.txt";
};

/* class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
}; */

#endif