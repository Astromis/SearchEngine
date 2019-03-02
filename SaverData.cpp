#include "SaverData.h"


/* void TextFileSaverData::save(InvertIndex *instance)
{
    string table = "table.txt", table_len = "table_len.txt";
    
    ofstream myfile;
    myfile.open (path_ + table);
    //myfile << "Writing this to a file.\n";
    for(auto i: (*instance).GetN2D())
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
    //return 0;
} */

/**
 * @brief Contructor
 * @param [in] path Path for saving a file
 */
TextFileSaverData::TextFileSaverData(string path)
{
    path_ = path; 
}


void TextFileSaverData::save(InvertIndex *instance)
{
    write_simple_map(instance->GetD2L(), table_len);

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