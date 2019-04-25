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

void save_index(inverted_list index)
{
    ofstream f1("index.bin", ios::binary);
    size_t temp_size, amount_of_doc = 0;
    //amount of words
    temp_size = index.size();
    f1.write((char*)&temp_size, sizeof(size_t));
    for(auto& i: index)
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

void TextFileSaverData::save(InvertIndex *instance)
{
    string s1 = "D2L.txt";
    string s2 = "num2doc";
    string s3 = "doc2num";
    string s4 = "other.txt";

    write_simple_map(instance->GetD2L(), table_len);
    write_simple_map(instance->GetN2D(), s2);
    write_simple_map(instance->GetD2N(), s3);

    save_index(instance->GetIndex());
    ofstream f(s4, ios::out);
    f<<instance->GetAverLen()<<endl;
    f<<instance->GetDocCount()<<endl;
    f.close();

}

inverted_list load_index()
{
    ifstream f1("index.bin", ios::binary);
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
    return test;
}

void TextFileSaverData::load(InvertIndex *instance)
{
    string s1 = "D2L.txt";
    string s2 = "num2doc";
    string s3 = "doc2num";
    string s4 = "other.txt";
    inverted_list tmp = load_index();
    instance->SetIndex(tmp);
    instance->SetN2D(read_is_map(s2));
    instance->SetD2N(read_si_map(s3));
    instance->SetD2L(read_if_map(table_len));
    ifstream f(path_ + s4);
    string tmpf, tmpl;
    f>>tmpf;
    f>>tmpl;
    instance->SetAvrDocLen(stof(tmpf));
    instance->SetDocCount(stol(tmpl));
    f.close();

}

/**
 * @brief Write a map to a file
 * @param [in] mapType type of a map
 * @param [in] map Map to be saved
 * @param [in] filename File where map will be saved
 */
template<class mapType>
void TextFileSaverData::write_simple_map(mapType map, string filename)
{
    ofstream myfile;
    myfile.open (path_ + filename, ios::out);
    for(auto i: map)
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
}

map<int, string > TextFileSaverData::read_is_map(string filename)
{
    ifstream file;
    map<int, string > tmp;
    string num, str;
    file.open(path_ + filename, ios::out);
    while(file.eof() != true)
    {
        file>>num;
        file>>str;
        tmp[stoi(num)] = str;
    }
    return tmp;
}

map<string, int > TextFileSaverData::read_si_map(string filename)
{
    ifstream file;
    map<string, int > tmp;
    string num, str;
    file.open(path_ + filename, ios::out);
    while(file.eof() != true)
    {
        file>>str;
        file>>num;
        tmp[str] = stoi(num);
    }
    return tmp;
}

map<int, float > TextFileSaverData::read_if_map(string filename)
{
    ifstream file;
    map<int, float > tmp;
    string numi, numf;
    file.open(path_ + filename, ios::out);
    while(file.eof() != true)
    {
        file>>numi;
        file>>numf;
        tmp[stoi(numi)] = stof(numf);
    }
    return tmp;
}
