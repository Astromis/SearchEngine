#ifndef SAVERDATA_H
#define SAVERDATA_H

#include "InvertIndex.h"
#include <iostream>
#include <fstream>

using namespace std;


class InvertIndex;

class SaverData
{
    public:
    SaverData();
    virtual ~SaverData();
    //SaverData(string path_to);
    
    virtual void save(InvertIndex *instance);
    string path_;
    protected:

    

};

class TextFileSaverData : public SaverData 
{
    
    
    public:
        TextFileSaverData(string path){path_ = path; };
        void save(InvertIndex *instance)  override;
        string path_;
};

class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
};

#endif