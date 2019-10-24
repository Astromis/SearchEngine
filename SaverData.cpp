#include "SaverData.h"


/**
 * @brief Contructor
 * @param [in] path Path for saving a file
 */
BinarySaverData::BinarySaverData(string path)
{
    dir_path = path; 
}

/**
 * @brief Save inverted index as binaty to a file
 * @param [in] inverted_list index to be saved
 * 
 * The writing has a next format:
 * amount of all word in index -> word -> amount of docs -> doc ->
 * amount of position -> p,p,p,... -> doc -> amount of position -> 
 * ... -> word -> ...
*/

void BinarySaverData::save_index(inverted_list index)
{
    ofstream f1(index_file, ios::binary);
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

/**
 * @brief General saver function that saces all InvertedIndex structure
 * @param [in] Pointer on structure. Typically "this"
 */
void BinarySaverData::save(InvertIndex *instance, string filename)
{

    write_simple_map(instance->GetD2L(), doc2len_file);
    write_simple_map(instance->GetN2D(), num2doc_file);
    write_simple_map(instance->GetD2N(), doc2num_file);
    save_index(instance->GetIndex());

    ofstream f(other_file, ios::out);
    f<<instance->GetAverLen()<<endl;
    f<<instance->GetDocCount()<<endl;
    f.close();

}

/**
 * @brief Load inverted index
 * @return loaded inverted index
 */
inverted_list BinarySaverData::load_index()
{
    ifstream f1(index_file, ios::binary);
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

/**
 * @brief General loader function that loads all InvertedIndex structure
 * @param [in] Pointer on structure. Typically "this"
 */
void BinarySaverData::load(InvertIndex *instance, string filename)
{
    //TODO: Try to make index loading in one step.
    inverted_list tmp = load_index();
    instance->SetIndex(tmp);
    instance->SetN2D(read_is_map(num2doc_file));
    instance->SetD2N(read_si_map(doc2num_file));
    instance->SetD2L(read_if_map(doc2len_file));

    ifstream f(other_file);
    string tmpf, tmpl;
    f>>tmpf;
    f>>tmpl;
    instance->SetAvrDocLen(stof(tmpf));
    instance->SetDocCount(stol(tmpl));
    f.close();

}
