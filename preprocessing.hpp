#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <string>
#include <codecvt>
#include "contrib/Stemming-ru/source/StemmerPorter.h"
#include <cwctype>


class Preprocessing
{
    StemmerPorter stemmer;
    std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
    string lower(string str);
    string remove_punct(string str);
    bool is_lower, is_punct_remove, is_stemming;

    public:
    Preprocessing(bool is_lower, bool is_punct_remove, bool is_stemming);
    Preprocessing& operator=(const Preprocessing& obj);
    Preprocessing();
    ~Preprocessing();
    void set_lower(bool);
    void set_punct_remove(bool);
    void set_stemming(bool);
    string process(string);

};

#endif