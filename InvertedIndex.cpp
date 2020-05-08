#include "InvertedIndex.h"
#include "SaverData.h"
#include <algorithm>
#include <utility>
#include <set>

#define VERBOSE 1


mutex mut;
int InvertedIndex::doc_id = 0;

//TODO: 
// make tokenization (boost?)
// what does program should do if amount of files would be much more then memory?
//      The one idea is block writing in file by numerical condition.
// Speed.Probably, it would be a good idea to create several instances of inverted index.
//      Each index is owned by separated threads.
//  By the way, writting in file could done threadable. Just use seek() to get a posobility writting data in one file by several threads.


/**
 * @brief Saves the inverted index
 * @param saver Object of SaverData famaly classess,
 */ 
void InvertedIndex::save(SaverData& saver, string dir_instance)
{
    saver.save(this, dir_instance);
    #ifdef VERBOSE
    cout<<"Saving complited"<<endl;
    #endif
}

/**
 * @brief Loads the inverted index
 * @param saver Object of SaverData famaly classess,
 */ 
void InvertedIndex::load(SaverData& saver, string dir_instance)
{
    saver.load(this, dir_instance);
}

void InvertedIndex::clear_index()
{
    index.clear();
    //inverted_list().swap(index);
}