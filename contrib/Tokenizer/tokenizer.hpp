#include <regex>
#include <vector>
#include <utility>


using namespace std;


class Tokenizer
{
    Tokenizer();
    vector<pair<wregex, wstring> > STARTING_QUOTES, ENDING_QUOTES, PUNCTUATION, PARENS_BRACKETS, CONVERT_PARENTHESES, DOUBLE_DASHES;


};
