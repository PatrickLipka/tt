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
    size_t command_end = input.find(" ");
    std::string command = input.substr(0,command_end);
    std::string argument = input.substr(command_end+1); 

    if (command == "ls"){
        if (command_end == std::string::npos){
            command_ls("",proj_list);
        }else{
            command_ls(argument,proj_list);
        }
    }else if (command == "start"){
        if (command_end == std::string::npos){
            command_start("",proj_list);
        }else{
            command_start(argument,proj_list);
        }
            
    }else if (command == "np"){
        if (command_end == std::string::npos){
            std::cout << "new project: Please specify project name." << std::endl;
        }else{
            command_np(argument,proj_list);
        }
    }else if (command == "nt"){
        if (command_end == std::string::npos){
            std::cout << "new task: Please specify task name." << std::endl;
        }else{
            command_nt(argument, proj_list);
        }
    }else if (command == "sp"){
        if (command_end == std::string::npos){
            std::cout << "switch project: Please specify project name." << std::endl;
        }else{
            command_sp(argument, proj_list);
        }
    }
}

// list projects/tasks
void command_ls(std::string input, ProjectList *proj_list){
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    if (str.length() == 0){
        // list projects
        std::string active_proj_str = proj_list->active_project->name;
        if(proj_list->active_project->num_tasks > 0) active_proj_str += "/" + proj_list->active_project->active_task->name;
        std::cout << "Active Project/Task: " << active_proj_str << std::endl;
        std::cout << std::endl;
        std::cout << "List of Projects:" << std::endl;
        for (int i=0; i<proj_list->num_projects; i++){
            std::cout << proj_list->projects[i].name << std::endl;
        }
        std::cout << std::endl;
    }else{
        // list tasks for given project
        Project *proj = proj_list->find_project_by_name(underscore_to_space(str));
        if(proj == NULL){
            std::cout << "Project " << str << " does not exists." << std::endl;
        }else{
            std::cout << "List of Tasks for Project " << proj->name << ":" << std::endl;
            for(int j=0; j<proj->num_tasks; j++){
                std::cout << proj->tasks[j].name << std::endl;
            }       
        }
    }
}

// start tracking of active project or specific project
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
        if (proj == NULL){
            std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
            return;
        }
        int id=proj->find_task_id_by_name(underscore_to_space(task_name));
        if(id >= 0){
            std::cout << "id=" << id << std::endl;
            proj->set_active_task(id);
            std::cout << "now active: "<< proj->active_task->name << std::endl;
            track(proj);
        }else{
            std::cout << "Task " << underscore_to_space(str) << " does not exist." << std::endl;
        }
    }
}

// create new project
void command_np(std::string name, ProjectList *proj_list){
    if (proj_list->find_project_id_by_name(underscore_to_space(name)) == -1){
        Project proj(name); 
        std::string auto_complete_str = space_to_underscore(name);
        autocomplete_names.push_back(auto_complete_str);
        proj_list->add_project(proj);
    }else{
        std::cout << "A project with the name " << name << " already exists." << std::endl;
    }
}

// create new task
void command_nt(std::string input, ProjectList *proj_list){
    // change active task and start tracking
    size_t place_of_slash = input.find("/");
    std::string proj_name = input.substr(0,place_of_slash);
    std::string task_name = input.substr(place_of_slash+1);
    Project *proj = proj_list->find_project_by_name(underscore_to_space(proj_name));
    if (place_of_slash == std::string::npos){
        proj = proj_list->active_project;
    }
    if (proj == NULL){
    std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
    return;
    }
    int id = proj->find_task_id_by_name(underscore_to_space(task_name));
    if (id < 0){
        Task task(task_name);
        proj->add_task(task);
        std::string auto_complete_str = space_to_underscore(proj->name+"/"+task_name);
        autocomplete_names.push_back(auto_complete_str);
    }else{
        std::string str = proj->name+"/"+task_name;
        std::cout << "Task " << str << " already exists." << std::endl;
    }
}

// switch project
void command_sp(std::string name, ProjectList *proj_list){
    int id = proj_list->find_project_id_by_name(underscore_to_space(name));
    if (id >= 0){
        proj_list->set_active_project(id);
        std::cout << "Switched to project " << underscore_to_space(name) << std::endl;
    }else{
        std::cout << "Project " << underscore_to_space(name) << " does not exist." << std::endl;
    }
}



