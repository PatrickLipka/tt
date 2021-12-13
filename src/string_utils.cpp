#include <string>
#include "string_utils.h"

std::string space_to_underscore(std::string str){
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == ' ') str[i] = '_';
    }
    return str;
}

std::string underscore_to_space(std::string str){
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == '_') str[i] = ' ';
    }
    return str;
}

std::string trim(const std::string& str)
{
    const char* WhiteSpace = " ";
    std::size_t start = str.find_first_not_of(WhiteSpace);
    std::size_t end = str.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : str.substr(start, end - start + 1);
}
