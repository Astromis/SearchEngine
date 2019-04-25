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
    virtual void load(InvertIndex *instance)=0;
    //virtual ~SaverData();
    string path_;
    protected:

};

class TextFileSaverData : public SaverData 
{
    
    public:
        TextFileSaverData(string path);
        void save(InvertIndex *instance)  override;
        void load(InvertIndex *instance) override;

        template<class mapType>
        void write_simple_map(mapType map, string file_name);

        map<int, string > read_is_map(string filename);
        map<string, int > read_si_map(string filename);
        map<int, float > read_if_map(string filename);

        string path_;
        string table = "table.txt", table_len = "table_len.txt";
        
};

/* class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
}; */

#endif