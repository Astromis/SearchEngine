#include "tokenizer.hpp"

Tokenizer::Tokenizer()
{

    make_pair(wregex(L"([«“‘„]|[`]+)"), ""); 
}