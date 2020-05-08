%module InvertIndex
%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"

namespace std {
   %template(vectord) vector<string>;
   %template() std::pair<float,string>;
   %template(PairVector) std::vector<std::pair<float,string> >;
};

%{
  #include "InvertIndex.h"
%}

%include "InvertIndex.h"

// useful notes about swig on stack overflow
// https://stackoverflow.com/questions/42834517/iterate-returned-vectorpairint-int-in-python-from-swig-bindings
// https://stackoverflow.com/questions/38111942/swig-argument-error-when-using-using-stdvector-in-python/38113826#38113826
// https://stackoverflow.com/questions/20125384/return-vectorpairint-int-from-c-method-to-python-list-of-tuples-using-sw
// for comlex data types might be useful to use typemaps - declarations of mapping datatypes from one language to another
//      https://stackoverflow.com/questions/32478649/how-to-apply-swig-output-typemaps-for-class-types-in-python
