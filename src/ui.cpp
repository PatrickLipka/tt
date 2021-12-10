#include <iostream>
#include <vector>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include "ui.h"
#include "project.h"
#include "track.h"
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

std::string underscore_to_space(std::string str){
    for(size_t i = 0; i < str.length(); i++){
        if(str[i] == '_') str[i] = ' ';
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

void parse_input(std::string input, ProjectList *proj_list){
    if(input.find("ls") != std::string::npos){
        if (input.length()>2){
            command_ls(input.substr(3),proj_list);
        }else{
            command_ls("",proj_list);
        }
    } else if(input.find("start") != std::string::npos){
        if (input.length()>5){
            command_start(input.substr(6),proj_list);
        }else{
            command_start("",proj_list);
        }
    }
}


void command_ls(std::string input, ProjectList *proj_list){
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    if (str.length() == 0){
        // list projects
        std::cout << "List of Projects:" << std::endl;
        for (int i=0; i<proj_list->num_projects; i++){
            std::cout << proj_list->projects[i].name << std::endl;
        }
    }else{
        // list tasks for given project
        Project *proj = proj_list->find_project_by_name(underscore_to_space(str));
        std::cout << "List of Tasks for Project " << proj->name << ":" << std::endl;
        for(int j=0; j<proj->num_tasks; j++){
            std::cout << proj->tasks[j].name << std::endl;
        }
    }
}

void command_start(std::string input, ProjectList *proj_list){
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    if (str.length() == 0){
        // start tracking for active task
        track(proj_list->active_project);
    }else{
        // change active task and start tracking
        int place_of_slash = str.find("/");
        std::string proj_name = str.substr(0,place_of_slash);
        std::string task_name = str.substr(place_of_slash+1);
        Project *proj = proj_list->find_project_by_name(underscore_to_space(proj_name));
        int id=proj->find_task_id_by_name(underscore_to_space(task_name));
        if(id >= 0){
            std::cout << "id=" << id << std::endl;
            proj->set_active_task(id);
            std::cout << "now active: "<< proj->active_task->name << std::endl;
            track(proj);
        }
    }
}
