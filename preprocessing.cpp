#include "preprocessing.hpp"

Preprocessing::Preprocessing()
{
    setlocale(LC_ALL, "");

}

Preprocessing::~Preprocessing()
{

}

Preprocessing::Preprocessing(bool is_lower, bool is_punct_remove, bool is_stemming)
{
    setlocale(LC_ALL, "");

    is_lower = is_lower;
    is_punct_remove = is_punct_remove;
    is_stemming = is_stemming;
}

void Preprocessing::set_lower(bool f)
{
    is_lower = f;
}

void Preprocessing::set_punct_remove(bool f)
{
    is_punct_remove = f;
}

void Preprocessing::set_stemming(bool f)
{
    is_stemming = f;
}

string Preprocessing::process(string str)
{
    wstring temp = converter.from_bytes(str);
    if(is_lower)
    {
        transform(temp.begin(), temp.end(), temp.begin(), towlower);
    } if (is_punct_remove)
    {
        temp.erase(std::remove_if(temp.begin(), temp.end(),
	    [](wchar_t ch) { return !::iswalnum(ch); }), temp.end());
    } if (is_stemming)
    {
        temp = stemmer.stemm(temp);
    }

    return converter.to_bytes(temp);
}

Preprocessing& Preprocessing::operator=(const Preprocessing& obj)
{
    is_lower = obj.is_lower;
    is_punct_remove = obj.is_punct_remove;
    is_stemming = obj.is_stemming;
    return *this;
}