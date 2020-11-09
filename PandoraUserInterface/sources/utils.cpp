#include "utils.h"


bool isNumeric(string s){
    char* end = nullptr;
    strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}

