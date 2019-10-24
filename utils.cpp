#include "utils.hpp"

/**
 * @brief Writes string to a binary file correctly
 * @param [in] f fstream object of file
 * @param [in] s string to be saved
 */

int write_string(ofstream &f, string s)
{
    const char *s_arr = s.c_str();
    size_t len = s.size();
    f.write((char*)&len, sizeof(size_t));
    f.write(s_arr, len);
    return 0;
}

/**
 * @brief Reads string from a binary file correctly
 * @param [in] f fstream object of file
 * @return readed string
 */

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

// Next funtions is needed to load maps. 
// Because we can't define map conteiner on the fly, having read type from file, 
// here is direct readers of requared type of maps
map<int, string > read_is_map(string filename)
{
    ifstream file;
    map<int, string > tmp;
    string num, str;
    file.open(filename, ios::out);
    while(file.eof() != true)
    {
        file>>num;
        file>>str;
        tmp[stoi(num)] = str;
    }
    return tmp;
}

map<string, int > read_si_map(string filename)
{
    ifstream file;
    map<string, int > tmp;
    string num, str;
    file.open(filename, ios::out);
    while(file.eof() != true)
    {
        file>>str;
        file>>num;
        tmp[str] = stoi(num);
    }
    return tmp;
}

map<int, float > read_if_map(string filename)
{
    ifstream file;
    map<int, float > tmp;
    string numi, numf;
    file.open(filename, ios::out);
    while(file.eof() != true)
    {
        file>>numi;
        file>>numf;
        tmp[stoi(numi)] = stof(numf);
    }
    return tmp;
}

/**
 * @brief get the free memory available in the Linux
 * @return free memory in kB, -1 if somthing went wrong
 */
int get_free_memory()
{
    FILE * pFile;
    int amount;
    char str[50];
    pFile = fopen ("/proc/meminfo","r");
    while (fscanf(pFile, "%s\t%d kB\n" , str, &amount) != EOF)
    {
        if(string(str) == "MemFree:"){
            fclose(pFile);
            return amount;
        }
    }
    fclose(pFile);
    return -1;
}