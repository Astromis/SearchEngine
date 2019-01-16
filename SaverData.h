#include "InvertIndex.h"

class SaverData
{
    public:
    SaverData();
    virtual ~SaverData();
    virtual void save(string path) = 0;
    
    private:
    InvertIndex index_p;

};

class TextFileSaverData : public SaverData 
{
    
    public:
      TextFileSaverData(InvertIndex &instance);
      virtual void save(string path);
};

class DatabaseSaverData : public SaverData 
{
    public:
  virtual void save(string path);
  
};