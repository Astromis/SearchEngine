#include "SaverData.h"
#include <sstream>

/**
 * @brief Contructor
 * @param [in] path Path for saving a file
 */
TextFileSaverData::TextFileSaverData(string path)
{
    path_ = path; 
}

void write_string(ofstream &f, string s)
{
    const char *s_arr = s.c_str();
    size_t len = s.size();
    f.write((char*)&len, sizeof(size_t));
    f.write(s_arr, len);
}

string read_string(ifstream &f)
{
    
    size_t len = 0;
    f.read((char*)&len, sizeof(size_t));
    char *s = new char [len];
    f.read(s, len);
    string tmp = string(s,len);
    delete [] s;
    return tmp; 
}

void TextFileSaverData::save(InvertIndex *instance)
{
    //write_simple_map(instance->GetD2L(), table_len);

    ofstream f1("1.txt", ios::binary);
    size_t temp_size, amount_of_doc = 0;
    //amount of words
    temp_size = instance->GetIndex().size();
    f1.write((char*)&temp_size, sizeof(size_t));
    for(auto& i: instance->GetIndex())
    {
        write_string(f1, i.first);
        //amount of documents
        amount_of_doc = i.second.size();
        f1.write((char*)&amount_of_doc, sizeof(size_t));
        for(auto& j: i.second)
        {
            f1.write((char*)&j.first,sizeof(int));
            //amount of possition
            temp_size = j.second.size();
            f1.write((char*)&temp_size,sizeof(size_t));
            for(auto& k: j.second)
            {
                f1.write((char*)&k,sizeof(int));
            }
        }
    }

}

void TextFileSaverData::load()
{
    ifstream f1("1.txt", ios::binary);
    inverted_list test;
    string word, tmp;
    int doc_id, pos_id;
    size_t word_count, vec_counter, doc_counter;
    f1.read((char*)&word_count, sizeof(size_t));
    for(int i=0;  i < word_count;i++)
    {
        //read the word
        word = read_string(f1);
        //read the document counter
        f1.read((char*)&doc_counter, sizeof(size_t));
        for(int j=0; j < doc_counter; j++)
        {
            f1.read((char*)&doc_id, sizeof(int));
            //read position vector size
            f1.read((char*)&vec_counter, sizeof(size_t));
            for(int k=0; k<vec_counter;k++)
            {
                f1.read((char*)&pos_id, sizeof(int));
                test[word][doc_id].push_back(pos_id);
            }

        }
    }
}

/**
 * @brief Write a map to a file
 * @param [in] mapType type of a map
 * @param [in] map Map to be saved
 * @param [in] file_name File where map will be saved
 */
template<class mapType>
void TextFileSaverData::write_simple_map(mapType map, string file_name)
{
    ofstream myfile;
    myfile.open (path_ + file_name, ios::out);
    for(auto i: map)
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
}