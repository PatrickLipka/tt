#include <fstream>
#include <iostream>
#include "util.h"

std::string get_date(){
    time_t t = time(NULL);
    tm *date = localtime(&t);
    int month = date->tm_mon+1;
    int year  = date->tm_year+1900;
    std::string month_str;
    if (month < 10){
        month_str = "0"+std::to_string(month);
    }else{
        month_str = std::to_string(month);
    }
    std::string date_str = std::to_string(year)+"-"+month_str;
    return date_str;
}

std::string get_last_date(){
    time_t t = time(NULL);
    tm *date = localtime(&t);
    int month = date->tm_mon;
    int year = date->tm_year+1900;

    if(month == 0){
        month = 12;
        year--;
    }
    
    std::string month_str;
    if (month < 10){
        month_str = "0"+std::to_string(month);
    }else{
        month_str = std::to_string(month);
    }
    std::string date_str = std::to_string(year)+"-"+month_str;
    return date_str;
}

void parse_config_file(std::string file_name, std::string *user_name, std::string *tracking_dir){
    std::ifstream f(file_name);
    if (!f){
        std::cout << "ABORT: Could not open config file " << file_name << "for reading." << std::endl;
        f.close();
        exit(1);
    }
    std::string line;
    while(std::getline(f,line)){
        size_t place_of_eq = line.find("=");
        std::string key = line.substr(0,place_of_eq);
        std::string val = line.substr(place_of_eq+1);
        if (key == "user_name") *user_name = val;
        if (key == "tracking_directory") *tracking_dir = val;
    }
    if (user_name->length() == 0 || tracking_dir->length() == 0){
        std::cout << "ABORT: Malformed config file " << file_name << std::endl;
        f.close();
        exit(1);
    }
    f.close();
}
