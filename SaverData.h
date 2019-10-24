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

    virtual void save(InvertIndex *instance, string filename)=0;
    virtual void load(InvertIndex *instance, string filename)=0;
    //virtual ~SaverData();
    string root_dir_path;
    protected:

};

class BinarySaverData : public SaverData 
{

    string index_file = "index.bin";
    string doc2len_file = "D2L.txt";
    string num2doc_file = "num2doc";
    string doc2num_file = "doc2num";
    string other_file = "other.txt";
    
    public:
        BinarySaverData();
        BinarySaverData(string path);
        void save(InvertIndex *instance, string filename)  override;
        void load(InvertIndex *instance, string filename) override;

        inverted_list load_index();
        void save_index(inverted_list index);

        void create_dir();
        bool check_dir();

        void SetRootDir();

        
};

/* class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
}; */

#endif