#include "InvertIndex.h"
#include <iostream>
#include <fstream>

class SaverData
{
    public:
    SaverData();
    SaverData(string path);
    virtual ~SaverData();
    virtual void save(InvertIndex *instance) = 0;
    InvertIndex index_p;
    string path_;
    protected:

    private:
    

};

class TextFileSaverData : public SaverData 
{
    
    
    public:
        TextFileSaverData(string path):SaverData(path){};
        virtual void save(InvertIndex *instance);
};

class DatabaseSaverData : public SaverData 
{
    public:
  //virtual void save(InvertIndex *instance);
  
};
