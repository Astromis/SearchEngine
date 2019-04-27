#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;


int write_string(ofstream &f, string s);
string read_string(ifstream &f);

template<class mapType>
void write_simple_map(mapType map, string filename);

map<int, string > read_is_map(string filename);
map<string, int > read_si_map(string filename);
map<int, float > read_if_map(string filename);


/**
 * @brief Write a map to a file
 * @param [in] mapType type of a map
 * @param [in] map Map to be saved
 * @param [in] filename File where map will be saved
 */
template<class mapType>
void write_simple_map(mapType map, string filename)
{
    ofstream myfile;
    myfile.open (filename, ios::out);
    for(auto i: map)
        myfile<<i.first<<" "<<i.second<<endl;
    myfile.close();
}


#endif