#include "InvertIndex.h"

class SaverData
{
    public:
    virtual void save() = 0;

};

class TextFileSaverData : public SaverData 
{
    public:
  virtual void save();
};

class DatabaseSaverData : public SaverData 
{
    public:
  virtual void save();
  
};