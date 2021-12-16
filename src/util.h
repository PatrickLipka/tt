#ifndef UTIL_H
#define UTIL_H

#define STRING(s) STR(s)
#define STR(s) #s
#include <string>

std::string get_date();
std::string get_last_date();
void parse_config_file(std::string file_name, std::string *user_name, std::string *tracking_dir);

#endif
