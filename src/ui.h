#ifndef UI_H
#define UI_H

#include <vector>
#include <string>
#include "project.h"

const int num_commands=10;
extern std::string command_names[num_commands];
extern std::vector<std::string> autocomplete_names;
char **tt_name_completion(const char* text, int start, int end);
char *tt_name_generator(const char *text, int state);
void init_autocomplete(ProjectList *proj_list);
std::string space_to_underscore(std::string);
#endif
