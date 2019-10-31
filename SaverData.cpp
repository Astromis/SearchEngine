#include "SaverData.h"


/**
 * @brief Contructor
 * @param [in] path Path for saving a file
 */
BinarySaverData::BinarySaverData(string path)
{
    root_dir_path = path;
    if(check_dir(root_dir_path.c_str()) == false)
        if(create_dir(root_dir_path.c_str()) == false)
        {
            cout<<"Can't create root directory for storing data"<<endl;
            exit(-1);
        }

}

bool check_dir(const char *dir_path)
{
    struct stat info;

    if( stat( dir_path, &info ) != 0 )
        return false;
    else if( info.st_mode & S_IFDIR ) 
        return true;
    else
        return false;
}

bool create_dir(const char *dir_path)
{
    if (mkdir(dir_path, 0777) == -1) 
        return false;
    else
        return true; 
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
void BinarySaverData::save_index(inverted_list index, string dir_instance)
{
    ofstream f1(root_dir_path + dir_instance + index_file, ios::binary);
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
void BinarySaverData::save(InvertIndex *instance, string dir_instance)
{

    if(check_dir((root_dir_path + dir_instance).c_str()) == false)
        if(create_dir(root_dir_path.c_str()) == false)
        {
            cout<<"Can't create directory for binary saving data"<<endl;
            exit(-1);
        }
    write_simple_map(instance->GetD2L(), root_dir_path + dir_instance + doc2len_file);
    write_simple_map(instance->GetN2D(), root_dir_path + dir_instance + num2doc_file);
    write_simple_map(instance->GetD2N(), root_dir_path + dir_instance + doc2num_file);
    save_index(instance->GetIndex(), root_dir_path + dir_instance + index_file);

    ofstream f(root_dir_path + dir_instance + other_file, ios::out);
    f<<instance->GetAverLen()<<endl;
    f<<instance->GetDocCount()<<endl;
    f.close();

}

/**
 * @brief Load inverted index
 * @return loaded inverted index
 */
inverted_list BinarySaverData::load_index(string dir_instance)
{
    ifstream f1(root_dir_path + dir_instance + index_file, ios::binary);
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
void BinarySaverData::load(InvertIndex *instance, string dir_instance)
{
    if(check_dir( (root_dir_path + dir_instance).c_str() ) == false)
    {
        cout<<"Can't find the dir with the data"<<endl;
        exit(-1);
    }
    inverted_list tmp = load_index(root_dir_path + dir_instance);
    instance->SetIndex(tmp);
    instance->SetN2D(read_is_map(root_dir_path + dir_instance + num2doc_file));
    instance->SetD2N(read_si_map(root_dir_path + dir_instance + doc2num_file));
    instance->SetD2L(read_if_map(root_dir_path + dir_instance + doc2len_file));

    ifstream f(root_dir_path + dir_instance + other_file);
    string tmpf, tmpl;
    f>>tmpf;
    f>>tmpl;
    instance->SetAvrDocLen(stof(tmpf));
    instance->SetDocCount(stol(tmpl));
    f.close();

}
