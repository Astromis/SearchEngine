%module InvertIndex
%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"

namespace std {
   %template(vectord) vector<string>;
};

%{
  #include "InvertIndex.h"
%}

%include "InvertIndex.h"
