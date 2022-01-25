#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include <string>

std::string trim(const std::string &str);
std::string space_to_underscore(std::string str);
std::string underscore_to_space(std::string str);
bool is_num(const std::string &str);
int string2sec(std::string str);
#endif
