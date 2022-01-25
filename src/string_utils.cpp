#include <string>
#include <iostream>
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

// convert string "X[s,m,h]" to seconds
int string2sec(std::string str){
    int seconds = 0;
    size_t place_of_hour = str.find("h");
    size_t place_of_minute = str.find("m");
    size_t place_of_second = str.find("s");
    std::string timestr=str;
    int factor = 1;
    
    if (is_num(str)){
        seconds = std::stoi(str);
        return seconds;
    }else if (place_of_hour < place_of_minute){
        if (place_of_hour < place_of_second){
            //seconds = std::stoi(str.substr(0,place_of_hour))*3600;
            timestr = str.substr(0,place_of_hour);
            factor = 3600;
        }else {
            //seconds = std::stoi(str.substr(0,place_of_second));
            timestr = str.substr(0,place_of_second);
        }
    }else if (place_of_minute < place_of_hour){
        if (place_of_minute < place_of_second){
            //seconds = std::stoi(str.substr(0,place_of_minute))*60;
            timestr = str.substr(0,place_of_minute);
            factor = 60;
        }else{
            //seconds = std::stoi(str.substr(0,place_of_second));
            timestr = str.substr(0,place_of_second);
        }
    }else{
        timestr = str.substr(0,place_of_second);
    }

    if (is_num(timestr) && timestr.length() > 0){
        seconds = std::stoi(timestr)*factor;
    } else{
            std::cout << "Wrong format of time string. Allowed: <X> OR <X>s OR <X>m OR <X>h." << std::endl;
    }
    return seconds;
}
