#include "SaverData.h"


/**
 * @brief Contructor
 * @param [in] path Path for saving a file
 */
BinarySaverData::BinarySaverData(string path)
{
    const_root_dir_path = path;
    if(fs::exists(const_root_dir_path) == false)
        if(create_directories(const_root_dir_path) == false)
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
 * @brief Save inverted index as binary to a file
 * @param [in] inverted_list index to be saved
 * 
 * The writing has a next format:
 * amount of all word in index -> word -> amount of docs -> doc ->
 * amount of position -> p,p,p,... -> doc -> amount of position -> 
 * ... -> word -> ...
*/
void BinarySaverData::save_index(inverted_list index, string file_path)
{
    ofstream f1(file_path, ios::binary);
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
    fs::path root_dir_path = const_root_dir_path;
    fs::path saving_path = root_dir_path.append(dir_instance);
    if(fs::exists(saving_path) == false)
        if(create_directories(saving_path) == false)
        {
            cout<<"Can't create root directory for saving data"<<endl;
            exit(-1);
        }

    write_simple_map(instance->GetD2L(), saving_path.append(doc2len_file));
    saving_path = saving_path.parent_path();

    write_simple_map(instance->GetN2D(), saving_path.append(num2doc_file));
    saving_path = saving_path.parent_path();

    write_simple_map(instance->GetD2N(), saving_path.append(doc2num_file));
    saving_path = saving_path.parent_path();
    cout<<saving_path<<endl;
    save_index(instance->GetIndex(), saving_path.append(index_file));
    saving_path = saving_path.parent_path();

    ofstream f(saving_path.append(other_file), ios::out);
    f<<instance->GetAverLen()<<endl;
    f<<instance->GetDocCount()<<endl;
    f.close();

}

/**
 * @brief Load inverted index
 * @return loaded inverted index
 */
inverted_list BinarySaverData::load_index(string file_path)
{
    ifstream f1(file_path, ios::binary);
    inverted_list test;
    string word, tmp;
    int doc_id, pos_id;
    size_t word_count, vec_counter, doc_counter;
    f1.read((char*)&word_count, sizeof(size_t));
    for(unsigned int i=0;  i < word_count;i++)
    {
        //read the word
        word = read_string(f1);
        //read the document counter
        f1.read((char*)&doc_counter, sizeof(size_t));
        for(unsigned int j=0; j < doc_counter; j++)
        {
            f1.read((char*)&doc_id, sizeof(int));
            //read position vector size
            f1.read((char*)&vec_counter, sizeof(size_t));
            for(unsigned int k=0; k<vec_counter;k++)
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
    fs::path root_dir_path = const_root_dir_path;

    fs::path loading_path = root_dir_path.append(dir_instance);
    if(fs::exists(loading_path) == false)
    {
        cout<<"Can't find the dir with the data "<<loading_path<<endl;
        exit(-1);
    }
    inverted_list tmp = load_index(loading_path.append(index_file));
    instance->SetIndex(tmp);
    loading_path = loading_path.parent_path();

    instance->SetN2D(read_is_map(loading_path.append(num2doc_file)));
    loading_path = loading_path.parent_path();

    instance->SetD2N(read_si_map(loading_path.append(doc2num_file)));
    loading_path = loading_path.parent_path();

    instance->SetD2L(read_if_map(loading_path.append(doc2len_file)));
    loading_path = loading_path.parent_path();

    ifstream f(loading_path.append(other_file));
    string tmpf, tmpl;
    f>>tmpf;
    f>>tmpl;
    instance->SetAvrDocLen(stof(tmpf));
    instance->SetDocCount(stol(tmpl));
    f.close();

}

IndexBuffer::IndexBuffer()
{
    file_handler.open("", ios::binary);
    
}
IndexBuffer::IndexBuffer(const IndexBuffer &obj)
{
    index = obj.index;
    index_it = obj.index_it; 
    index_it_end = obj.index_it_end;
    index_file = obj.index_file;
    file_handler.open(obj.file_name, ios::binary);
    word = obj.word;
    word_count =  obj.word_count;
    load_counter = obj.load_counter;
}

IndexBuffer::IndexBuffer(string file_path)
{
    file_handler.open(file_path, ios::binary);
    if(file_handler.is_open() != true)
    {
        cout<<"Couldn't open file"<<endl;
        exit(1);
    }
    // need for copy constructor
    file_name = file_path;
    file_handler.read((char*)&word_count, sizeof(size_t));
}

string IndexBuffer::get_top_word()
{
    return index_it->first;
}

word_position_map IndexBuffer::get_top_position_map()
{
    return index_it->second;
}

int IndexBuffer::next()
{
    if(index_it != index_it_end)
    {
        index_it++;
        return 0;
    }
    else
    {
        index.clear();
        if(load_counter < word_count)
        {
            load_portion( word_count-load_counter > 10 ? 10 : word_count-load_counter);
            return 0;
        }
        else return 1;
    }

}

void IndexBuffer::load_portion(int amount)
{
    int doc_id, pos_id;
    size_t vec_counter, doc_counter;
    for(int i=0;  i < amount;i++)
    {
        //read the word
        word = read_string(file_handler);
        //read the document counter
        file_handler.read((char*)&doc_counter, sizeof(size_t));
        for(unsigned int j=0; j < doc_counter; j++)
        {
            file_handler.read((char*)&doc_id, sizeof(int));
            //read position vector size
            file_handler.read((char*)&vec_counter, sizeof(size_t));
            for(unsigned int k=0; k<vec_counter;k++)
            {
                file_handler.read((char*)&pos_id, sizeof(int));
                index[word][doc_id].push_back(pos_id);
            }
        }
    }
    load_counter += amount;
    index_it = index.begin();
    index_it_end = index.end();
}