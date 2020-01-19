#ifndef SAVERDATA_H
#define SAVERDATA_H

#include "InvertIndex.h"
#include <iostream>
#include <fstream>
#include "utils.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <bits/stdc++.h> 
#include <experimental/filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;

bool check_dir(const char *dir_path);
bool create_dir(const char *dir_path);

class InvertIndex;

class SaverData
{
    public:

    virtual void save(InvertIndex *instance, string file_prefix)=0;
    virtual void load(InvertIndex *instance, string file_prefix)=0;
    //virtual ~SaverData();
    fs::path const_root_dir_path;
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
        void save(InvertIndex *instance, string file_prefix)  override;
        void load(InvertIndex *instance, string file_prefix) override;

        inverted_list load_index(string dir_instance);
        void save_index(inverted_list index, string dir_instance);

        

        void SetRootDir();

        
};

class IndexBuffer
{
    public:
        inverted_list index;
        inverted_list::iterator index_it, index_it_end;
        string index_file;
        ifstream file_handler;
        string word, file_name;
        size_t word_count, load_counter;
        bool allow; // not used
        void load_portion(int amount);

    public:
        IndexBuffer(const IndexBuffer &);
        IndexBuffer();
        IndexBuffer(string file_path);
        string get_top_word();
        word_position_map get_top_position_map();
        int next();
        bool check_allow(); // not used

};

/* class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
}; */

#endif