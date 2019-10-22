#ifndef SAVERDATA_H
#define SAVERDATA_H

#include "InvertIndex.h"
#include <iostream>
#include <fstream>
#include "utils.hpp"

using namespace std;


class InvertIndex;

class SaverData
{
    public:

    virtual void save(InvertIndex *instance)=0;
    virtual void load(InvertIndex *instance)=0;
    //virtual ~SaverData();
    string dir_path;
    protected:

};

class BinarySaverData : public SaverData 
{
    
    public:
        BinarySaverData(string path);
        void save(InvertIndex *instance)  override;
        void load(InvertIndex *instance) override;

        inverted_list load_index();
        void save_index(inverted_list index);

        string dir_path;
        string index_file = dir_path + "index.bin";
        string doc2len_file = dir_path + "D2L.txt";
        string num2doc_file = dir_path + "num2doc";
        string doc2num_file = dir_path + "doc2num";
        string other_file = dir_path + "other.txt";
        
};

/* class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
}; */

#endif