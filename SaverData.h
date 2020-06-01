#ifndef SAVERDATA_H
#define SAVERDATA_H

#include "InvertedIndex.h"
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

    virtual void save(InvertedIndex *instance, string file_prefix)=0;
    virtual void load(InvertedIndex *instance, string file_prefix)=0;
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
        void save(InvertedIndex *instance, string file_prefix)  override;
        void load(InvertedIndex *instance, string file_prefix) override;

        inverted_list load_index(string dir_instance);
        void save_index(inverted_list index, string dir_instance);

        

        void SetRootDir();

        
};

/**
 * Class that allows to read index partitially. Used in large scale indexing
 */ 
class IndexBuffer
{
    public:
        inverted_list index;
        inverted_list::iterator index_it, index_it_end;
        ifstream file_handler;
        string word, index_file;
        size_t word_count, load_counter;
        void load_portion(int amount);
        int load_amount;

        streampos position;

    public:
        IndexBuffer(const IndexBuffer &);
        IndexBuffer();
        IndexBuffer(string file_path, int amount=100);
        string get_top_word();
        word_position_map get_top_position_map();
        int next();
        inverted_list GetInvertedIndex();
        IndexBuffer& operator=(const IndexBuffer& obj);

};

/**
 * Class that allows to store index partitially. Used in large scale indexing
 */ 
class IndexBufferO
{
    public:
    inverted_list index;
    ofstream file_handler;
    string word, index_file;
    size_t word_count, save_counter, _capacity;
    int save_amount;
    streampos position;
    int total_words;
    inverted_list::iterator index_it, index_it_end;

    IndexBufferO();
    ~IndexBufferO();
    IndexBufferO(const IndexBufferO &);
    IndexBufferO(string file_path, int amount=100);
    void save_portion();
    int next();
    IndexBuffer& operator=(const IndexBuffer& obj);
    word_position_map& operator[](string q);
};
/* class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
}; */

#endif