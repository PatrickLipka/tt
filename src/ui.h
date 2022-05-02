#ifndef UI_H
#define UI_H

// define terminal format codes
#ifdef FORMATTED_TEXT
#define TERM_BOLD "\033[1m"
#define TERM_RESET "\033[0m"
#else
#define TERM_BOLD ""
#define TERM_RESET ""
#endif

#include <vector>
#include <string>
#include "project.h"

const int num_commands=15;
extern std::string command_names[num_commands];
extern std::vector<std::string> autocomplete_names;
char **tt_name_completion(const char* text, int start, int end);
char *tt_name_generator(const char *text, int state);
void init_autocomplete(ProjectList *proj_list);
std::string space_to_underscore(std::string);
std::string underscore_to_space(std::string);

void parse_input(std::string input, ProjectList *proj_list);

// command definitions
void command_ls(std::string input, ProjectList *proj_list);
void command_start(std::string input, ProjectList *proj_list);
void command_np(std::string name, ProjectList *proj_list);
void command_nt(std::string input, ProjectList *proj_list);
void command_rm(std::string input, ProjectList *proj_list);
void command_sp(std::string name, ProjectList *proj_list);
void command_st(std::string input, ProjectList *proj_list);
void command_nt(std::string input, ProjectList *proj_list);
void command_re(std::string input, std::string new_name,  ProjectList *proj_lits);
void command_at(std::string input, int wtime, ProjectList *proj_list);
void command_rt(std::string input, int wtime, ProjectList *proj_list);
void command_report(std::string date_str, ProjectList *proj_list);
void command_save(ProjectList *proj_list);
void command_load(std::string date_str, ProjectList *proj_list);
void command_convert(std::string date_str);
void command_version();
#endif
