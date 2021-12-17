#include <string>
#include "string_utils.h"

// replaces white spaces by underscores, used for readline completion
std::string space_to_underscore(std::string str){
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == ' ') str[i] = '_';
    }
    return str;
}

// replaces underscores by white space, used to translate readline completion to real names
std::string underscore_to_space(std::string str){
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == '_') str[i] = ' ';
    }
    return str;
}

// remove trailing and leading white spaces from string
std::string trim(const std::string& str){
    const char* WhiteSpace = " ";
    std::size_t start = str.find_first_not_of(WhiteSpace);
    std::size_t end = str.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : str.substr(start, end - start + 1);
}

// check whether string is a number
bool is_num(const std::string &str){
    for (const char &c : str){
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
