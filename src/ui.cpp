#include <iostream>
#include <vector>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include "ui.h"
#include "project.h"

// Use GNU readline library for autocompletion and command history

std::string command_names[num_commands]={
    "np",
    "nt",
    "sp",
    "st",
    "at",
    "rt",
    "rm",
    "report",
    "ls",
    "start"
};

std::vector<std::string> autocomplete_names;
char **tt_name_completion(const char* text, int start, int end){
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, tt_name_generator);
}

char *tt_name_generator(const char *text, int state){
    static int idx, len;
    const char* name;

    if (!state){
        idx = 0;
        len = strlen(text);
    }

    while ((name = autocomplete_names[idx++].c_str())){
        if (strncmp(name, text, len) == 0){
            return strdup(name);

        }
    }
    return NULL;
}

std::string space_to_underscore(std::string str){
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == ' ') str[i] = '_';
    }
    return str;
}

void init_autocomplete(ProjectList *proj_list){
    // add command names to auto complete list
    for (int i=0; i<num_commands; i++){
        autocomplete_names.push_back(command_names[i]);
    }

    // add project/task names to auto complete list
    for (int i=0; i<proj_list->num_projects; i++){
        autocomplete_names.push_back(space_to_underscore(proj_list->projects[i].name));
        for (int j=0; j<proj_list->projects[i].num_tasks; j++){
            
            std::string tmp = space_to_underscore(proj_list->projects[i].name) + "/" + space_to_underscore(proj_list->projects[i].tasks[j].name);
            autocomplete_names.push_back(tmp);
        }
    }
}
