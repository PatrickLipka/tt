#ifndef TT_H
#define TT_H
#define STRING(s) STR(s)
#define STR(s) #s

#include <string>
extern std::string user_name;
extern std::string tracking_dir;
extern std::string config_file;

std::string get_date();
void parse_config_file(std::string file_name, std::string *user_name, std::string *tracking_dir);
#endif
