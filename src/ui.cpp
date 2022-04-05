#include <iostream>
#include <vector>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iterator>
#include <unistd.h>
#include "ui.h"
#include "project.h"
#include "track.h"
#include "string_utils.h"
#include "tt.h"
#include "util.h"

// names of available commands
std::string command_names[num_commands]={
    "np",
    "nt",
    "sp",
    "st",
    "at",
    "rt",
    "re",
    "rm",
    "report",
    "ls",
    "start",
    "save",
    "load",
    "convert",
    "version"
};

std::vector<std::string> autocomplete_names; // global variable

// completion function for readline
char **tt_name_completion(const char* text, int start, int end){
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, tt_name_generator);
}

// search for input in auto complete names to attempt completion and give options
char *tt_name_generator(const char *text, int state){
     static std::vector<std::string>::const_iterator it;
     if (state == 0) it=begin(autocomplete_names);
     while(it != end(autocomplete_names)){
         std::string result = *it;
         ++it;
         if (result.find(text) != std::string::npos){
             return strdup(result.c_str()); 
         }
     }
     return NULL;
}

// add command and project/task names to auto complete list
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

// parse user input and call appropriate commad_* functions 
void parse_input(std::string input, ProjectList *proj_list){
    input = trim(input);
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
    }else if (command == "rm"){
        if (command_end == std::string::npos){
            std::cout << "remove: Please specify project or task name." << std::endl;
        }else{
            command_rm(argument,proj_list);
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
    }else if (command == "st"){
        if (command_end == std::string::npos){
            std::cout << "switch task: Please specify task name." << std::endl;
        }else{
            command_st(argument, proj_list);
        }
    }else if (command == "re"){
        if (command_end == std::string::npos){
            std::cout << "rename: Please specify project or  task name." << std::endl;
        }else{
            size_t first_name_end = argument.find(" ");
            command_re(argument.substr(0,first_name_end),argument.substr(first_name_end+1),proj_list);
        }
    }else if (command == "at"){
        int seconds = 0;
        if (command_end == std::string::npos){
            std::cout << "add time: please specify time to add and task name [optional]." << std::endl;
        }else{
            // only 1 argument
            if(argument.find(" ") == std::string::npos){
                seconds = string2sec(argument);
                command_at("",seconds,proj_list);
            }else{
                size_t start_of_name = argument.find(" ");
                std::string wtime_str = argument.substr(0,start_of_name);
                std::string task_name = argument.substr(start_of_name+1);
                seconds = string2sec(wtime_str);
                if (start_of_name != std::string::npos){
                    command_at(task_name,seconds,proj_list);
                }else{
                    std::cout << "add time: please specify time to add and task name [optional]." << std::endl;
                }

            }
        }
        
    }else if (command == "rt"){
        int seconds = 0;
        if (command_end == std::string::npos){
            std::cout << "remove time: please specify time to remove and task name [optional]." << std::endl;
        }else{
            // only 1 argument
            if(argument.find(" ") == std::string::npos){
                seconds = string2sec(argument);
                command_rt("",seconds,proj_list);
            }else{
                size_t start_of_name = argument.find(" ");
                std::string wtime_str = argument.substr(0,start_of_name);
                std::string task_name = argument.substr(start_of_name+1);
                seconds = string2sec(wtime_str);
                if (start_of_name != std::string::npos){
                    command_rt(task_name,seconds,proj_list);
                }else{
                    std::cout << "remove time: please specify time to remove and task name [optional]." << std::endl;
                }                                                                                       
            }
        }
    }else if (command == "report"){
        if(command_end == std::string::npos){
            command_report("",proj_list);
        }else{
            command_report(argument,proj_list);
        }
    }else if (command == "save"){
        command_save(proj_list);
    }else if (command == "load"){
        if(command_end == std::string::npos){
            std::cout << "load: please specify tracking file to load (format: yyyy-mm)" << std::endl;
        }else{
            command_load(argument,proj_list);
        }
    }else if (command == "convert"){
        if(command_end == std::string::npos){
            std::cout << "convert: please specify tracking file to convert (format: yyyy-mm)" << std::endl;
        }else{
            command_convert(argument);
        }

    }else if (command == "version"){
        command_version();
    }
}

// list projects/tasks
void command_ls(std::string input, ProjectList *proj_list){
    std::string active_proj_str="";
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    if (str.length() == 0){
        // list projects
        if(proj_list->num_projects > 0) active_proj_str = proj_list->active_project->name;
        if(proj_list->num_projects > 0 && proj_list->active_project->num_tasks > 0) active_proj_str += "/" + proj_list->active_project->active_task->name;
        std::cout << "Active Project/Task: " << active_proj_str << std::endl;
        std::cout << std::endl;
        std::cout << "List of Projects:" << std::endl;
        for (int i=0; i<proj_list->num_projects; i++){
            std::cout << proj_list->projects[i].name << std::endl;
        }
        std::cout << std::endl;
    }else{
        // list tasks for given project
        Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(str)));
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
        if (proj_list->active_project == NULL || proj_list->active_project->active_task == NULL){
            std::cout << "Project list or current project is empty. Please add projects/tasks first." << std::endl;
        }else{
            track(proj_list->active_project);
            command_save(proj_list);
        }       
    }else{
        // change active task and start tracking
        int place_of_slash = str.find("/");
        std::string proj_name = str.substr(0,place_of_slash);
        std::string task_name = str.substr(place_of_slash+1);
        Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
        if (proj == NULL){
            std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
            return;
        }
        int id=proj->find_task_id_by_name(trim(underscore_to_space(task_name)));
        if(id >= 0){
            proj->set_active_task(id);
            track(proj);
            command_save(proj_list);
        }else{
            std::cout << "Task " << underscore_to_space(str) << " does not exist." << std::endl;
        }
    }
}

// create new project
void command_np(std::string name, ProjectList *proj_list){
    if (proj_list->find_project_id_by_name(trim(underscore_to_space(name))) == -1){
        Project proj(underscore_to_space(name)); 
        std::string auto_complete_str = space_to_underscore(name);
        autocomplete_names.push_back(auto_complete_str);
        proj_list->add_project(proj);
        command_sp(name, proj_list);
    }else{
        std::cout << "A project with the name " << name << " already exists." << std::endl;
    }
}

// remove project/task
void command_rm(std::string input, ProjectList *proj_list){
    size_t place_of_slash = input.find("/");
    std::string proj_name = input.substr(0,place_of_slash);
    std::string task_name = input.substr(place_of_slash+1);
    int id = proj_list->find_project_id_by_name(trim(underscore_to_space(proj_name)));
    Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
    if (proj == NULL){
        proj = proj_list->active_project;
    }
    bool del = false;
    if (place_of_slash == std::string::npos && id >= 0){
        // do not let the user remove projects by accident
        std::cout << "Do you really want to remove project " << underscore_to_space(proj_name) << "? [y|n]" << std::endl;
        std::string input;
        while(1){
            std::cin >> input;
            if (input == "Y" || input == "y"){
                del = true;
                break;
            }else if( input == "n" || input == "N" ){
                del = false;
                break;
            }else{
                std::cout << "[y|n]? ";
            }
        }
        if(del){ 
            // also remove project with all tasks from auto completion
            auto find_id = std::find(autocomplete_names.begin(), autocomplete_names.end(),space_to_underscore(proj_name));
            if (find_id != autocomplete_names.end()){
                autocomplete_names.erase(find_id);
                for (int i=0; i<proj->num_tasks; i++){
                    std::string tmp = space_to_underscore(proj->name)+"/"+space_to_underscore(proj->tasks[i].name);
                    
                    auto find_task_id = std::find(autocomplete_names.begin(), autocomplete_names.end(),tmp);
                    if (find_task_id !=  autocomplete_names.end()){
                        autocomplete_names.erase(find_task_id);
                    }
                }
            }
            proj_list->remove_project(id);
            std::cout << "Removed project " << underscore_to_space(proj_name) << "." << std::endl;

            if (proj_list->num_projects > 0) {
                std::cout << "Switched to project " << proj_list->active_project->name << std::endl;
             }
            command_save(proj_list);
        }        
    }else{
        int task_id = proj_list->active_project->find_task_id_by_name(trim(underscore_to_space(task_name)));
        if (task_id >=0){
            del = false;
            // do not let the user remove tasks by accident
            std::cout << "Do you really want to remove task " << underscore_to_space(proj->name) << "/" << underscore_to_space(task_name) << "? [y|n]" << std::endl;
            std::string input;
            while(1){
                std::cin >> input;
                if (input == "Y" || input == "y"){
                    del = true;
                    break;
                }else if( input == "n" || input == "N" ){
                    del = false;
                    break;
                }else{
                    std::cout << "[y|n]? ";
                }
            }
            if(del){
                proj->remove_task(task_id);

                // also remove task from auto completion
                std::string tmp = space_to_underscore(proj->name)+"/"+space_to_underscore(task_name);
                auto find_id = std::find(autocomplete_names.begin(), autocomplete_names.end(),tmp);
                if (find_id !=  autocomplete_names.end()){
                    autocomplete_names.erase(find_id);
                }

                std::cout << "Removed  task " << underscore_to_space(proj->name) << "/" << underscore_to_space(task_name) << std::endl;
                if (proj->num_tasks > 0) {
                    std::cout << "Switched to task " << underscore_to_space(proj->name) << "/" << underscore_to_space(proj->active_task->name) << std::endl;
                 }
                command_save(proj_list);
            }        
        }else{
            std::cout << "Task " << underscore_to_space(proj->name) << "/" << underscore_to_space(task_name) << " does not exist." << std::endl;
        }
    }
}

// create new task
void command_nt(std::string input, ProjectList *proj_list){
    size_t place_of_slash = input.find("/");
    std::string proj_name = input.substr(0,place_of_slash);
    std::string task_name = input.substr(place_of_slash+1);
    Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
    if (place_of_slash == std::string::npos){
        proj = proj_list->active_project;
    }
    if (proj == NULL){
        std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
        return;
    }
    int id = proj->find_task_id_by_name(trim(underscore_to_space(task_name)));
    if (id < 0){
        Task task(task_name);
        proj->add_task(task);
        std::string auto_complete_str = space_to_underscore(proj->name+"/"+task_name);
        autocomplete_names.push_back(auto_complete_str);
        command_st(auto_complete_str, proj_list);
    }else{
        std::string str = underscore_to_space(proj->name)+"/"+underscore_to_space(task_name);
        std::cout << "Task " << str << " already exists." << std::endl;
    }
}

// switch project
void command_sp(std::string name, ProjectList *proj_list){
    int id = proj_list->find_project_id_by_name(trim(underscore_to_space(name)));
    if (id >= 0){
        proj_list->set_active_project(id);
        std::cout << "Switched to project " << underscore_to_space(name) << std::endl;
    }else{
        std::cout << "Project " << underscore_to_space(name) << " does not exist." << std::endl;
    }
}

// switch task
void command_st(std::string input, ProjectList *proj_list){
   size_t place_of_slash = input.find("/");
   std::string proj_name = input.substr(0,place_of_slash);
   std::string task_name = input.substr(place_of_slash+1);
   int proj_id = proj_list->find_project_id_by_name(trim(underscore_to_space(proj_name)));
   Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
   if (place_of_slash == std::string::npos){
        // task in same project
        proj = proj_list->active_project;
        proj_id = proj_list->find_project_id_by_name(trim(underscore_to_space(proj_list->active_project->name)));
   }
   if (proj == NULL){
        std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
        return;
   }
   int task_id = proj->find_task_id_by_name(trim(underscore_to_space(task_name)));
   if (task_id >= 0){
        proj_list->set_active_project(proj_id);
        proj->set_active_task(task_id);
        std::cout << "Switched to task " << proj_list->active_project->name << "/" << proj_list->active_project->active_task->name << std::endl;
   }else{
       std::string str = underscore_to_space(proj->name)+"/"+underscore_to_space(task_name);
       std::cout << "Task " << str << " does not exist." << std::endl;
   }
}

// rename project / task
void command_re(std::string input, std::string new_name, ProjectList *proj_list){
    size_t place_of_slash = input.find("/");
    std::string proj_name = input.substr(0,place_of_slash);
    std::string task_name = input.substr(place_of_slash+1);
    int id = proj_list->find_project_id_by_name(trim(underscore_to_space(proj_name)));
    Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
    if (proj == NULL){
        proj = proj_list->active_project;
    }
    if (place_of_slash == std::string::npos && id >= 0){
        // also edit auto completion
        auto find_id = std::find(autocomplete_names.begin(), autocomplete_names.end(),space_to_underscore(proj_name));
        if (find_id != autocomplete_names.end()){
            autocomplete_names.erase(find_id);
            autocomplete_names.push_back(space_to_underscore(new_name));
             // also do rename for tasks
            for (int i=0; i<proj->num_tasks; i++){
                std::string tmp = space_to_underscore(proj->name)+"/"+space_to_underscore(proj->tasks[i].name);
                std::string new_tmp = space_to_underscore(new_name)+"/"+space_to_underscore(proj->tasks[i].name);
                auto find_task_id = std::find(autocomplete_names.begin(), autocomplete_names.end(),tmp);
                if (find_task_id !=  autocomplete_names.end()){
                    autocomplete_names.erase(find_task_id);
                    autocomplete_names.push_back(new_tmp);
                }
            }
        }
        std::string old_name = proj_list->projects[id].name;
        proj->name = underscore_to_space(new_name);
        std::cout << "Renamed  project " << old_name << " to " << proj->name << std::endl;
        command_save(proj_list);
    }else{
        int task_id = proj_list->active_project->find_task_id_by_name(trim(underscore_to_space(task_name)));
        if (task_id >=0){
            proj->tasks[task_id].name = underscore_to_space(new_name);
                
            // also edit auto completion
            std::string tmp = space_to_underscore(proj->name)+"/"+space_to_underscore(task_name);
            auto find_id = std::find(autocomplete_names.begin(), autocomplete_names.end(),tmp);
            if (find_id !=  autocomplete_names.end()){
                autocomplete_names.erase(find_id);
                autocomplete_names.push_back(space_to_underscore(proj->name)+"/"+space_to_underscore(new_name));
            }
            std::cout << "Renamed task " << underscore_to_space(proj->name) << "/" << underscore_to_space(task_name) << " to " << underscore_to_space(proj->name) << "/" << underscore_to_space(new_name) << std::endl;
            command_save(proj_list);
        }else{
            std::cout << "Task " << underscore_to_space(proj->name) << "/" << underscore_to_space(task_name) << " does not exist." << std::endl;
        }
    }
}


// add time to task
void command_at(std::string input, int wtime, ProjectList *proj_list){
    size_t place_of_slash = input.find("/");
    std::string proj_name = input.substr(0,place_of_slash);
    std::string task_name = input.substr(place_of_slash+1);
    Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
    if(place_of_slash == std::string::npos){
        if(proj_name.length()==0){
            // add time to active task
            if(proj_list->active_project->active_task == NULL){
                std::cout << "Task " << proj_list->active_project->name << "/" << proj_name << " does not exist." << std::endl;
                return ;
            }
            proj_list->active_project->active_task->add_time(wtime);
            std::cout << wtime << "s added to task " << proj_list->active_project->name << "/" << proj_list->active_project->active_task->name  << std::endl;
            command_save(proj_list);
            return;
        }else{
            // add time to task in same project
            int id=proj_list->active_project->find_task_id_by_name(trim(underscore_to_space(proj_name)));
            if (id >= 0){
                proj_list->active_project->tasks[id].add_time(wtime);
                std::cout << wtime << "s added to task " << proj_list->active_project->name << "/" << proj_list->active_project->tasks[id].name << std::endl;
                command_save(proj_list);
                return;
            }else{
                std::cout << "Task " << proj_list->active_project->name << "/" << proj_name << " does not exist." << std::endl;
                return;
            }
                    
        }
    }
    if (proj == NULL){
        std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
        return;
    }
    int id=proj->find_task_id_by_name(trim(underscore_to_space(task_name)));
    if(id >= 0){
        proj->tasks[id].add_time(wtime);
        std::cout << wtime << "s added to task " << proj->name << "/" << proj->tasks[id].name << std::endl;
        command_save(proj_list);
    }else{
        std::cout << "Task " << underscore_to_space(input) << " does not exist." << std::endl;
    }
}

// remove time to task
void command_rt(std::string input, int wtime, ProjectList *proj_list){
    size_t place_of_slash = input.find("/");
    std::string proj_name = input.substr(0,place_of_slash);
    std::string task_name = input.substr(place_of_slash+1);
    Project *proj = proj_list->find_project_by_name(trim(underscore_to_space(proj_name)));
    if(place_of_slash == std::string::npos){
        if(proj_name.length()==0){
            // add time to active task
            if(proj_list->active_project->active_task == NULL){
                std::cout << "Task " << proj_list->active_project->name << "/" << proj_name << " does not exist." << std::endl;
                return ;
            }
            proj_list->active_project->active_task->add_time(-1*wtime);
            std::cout << wtime << "s removed from task " << proj_list->active_project->name << "/" << proj_list->active_project->active_task->name  << std::endl;
            command_save(proj_list);
            return;
        }else{
            int id=proj_list->active_project->find_task_id_by_name(trim(underscore_to_space(proj_name)));
            if (id >= 0){
                proj_list->active_project->tasks[id].add_time(-1*wtime);
                std::cout << wtime << "s removed from task " << proj_list->active_project->name << "/" << proj_list->active_project->tasks[id].name << std::endl;
                command_save(proj_list);
                return;
            }else{
                std::cout << "Task " << proj_list->active_project->name << "/" << proj_name << " does not exist." << std::endl;
                return;
            }
                    
        }
    }
    if (proj == NULL){
        std::cout << "Project " << underscore_to_space(proj_name) << " does not exist." << std::endl;
        return;
    }
    int id=proj->find_task_id_by_name(trim(underscore_to_space(task_name)));
    if(id >= 0){
        proj->tasks[id].add_time(-1*wtime);
        std::cout << wtime << "s removed from task " << proj->name << "/" << proj->tasks[id].name << std::endl;
        command_save(proj_list);
    }else{
        std::cout << "Task " << underscore_to_space(input) << " does not exist." << std::endl;
    }
}

// print monthly report 
void command_report(std::string date_str, ProjectList* proj_list){
    date_str = trim(date_str);
    size_t place_of_minus = date_str.find("-");
    std::string year = date_str.substr(0,place_of_minus);
    std::string month = date_str.substr(place_of_minus+1);

    if(date_str.length() == 0){
        // report for current month
        std::string date = proj_list->month; //get_date();
        std::cout << "Report for " << user_name << ", month: " << date << std::endl << std::endl;
        
        for (int i=0; i<proj_list->num_projects; i++){
            // compute task and total times
            Project *proj = &(proj_list->projects[i]);
            float wtime_proj = proj->get_total_work_time() / 3600.0;
            if(wtime_proj >= 0.01){
                std::cout << "Project: " << TERM_BOLD << proj->name << TERM_RESET << std::endl;
                for(int j=0; j<proj->num_tasks; j++){
                    float wtime_task = proj->tasks[j].work_time / 3600.0;
                    if(wtime_task >= 0.01) std::cout << "--- " << proj->tasks[j].name << ": " << std::fixed << std::setprecision(2) << wtime_task << std::endl;
                }
                // format: *.xx hours
                std::cout << TERM_BOLD << "Total: " << std::fixed << std::setprecision(2) << wtime_proj << TERM_RESET << std::endl << std::endl;
            }
        }
    }else{
        ProjectList list(month);
        std::string file_name = tracking_dir+"/"+date_str;
        list.load(file_name);
        if (list.num_projects > 0){
            std::cout << "Report for " << user_name << ", month: " << date_str <<  std::endl << std::endl;
            for (int i=0; i<list.num_projects; i++){
                // compute task and total times
                Project *proj = &(list.projects[i]);
                float wtime_proj = proj->get_total_work_time() / 3600.0;
                if(wtime_proj >= 0.01){
                    std::cout << "Project: " << TERM_BOLD << proj->name << TERM_RESET << std::endl;
                    for(int j=0; j<proj->num_tasks; j++){
                        float wtime_task = proj->tasks[j].work_time / 3600.0;
                        if(wtime_task >= 0.01) std::cout << "--- " << proj->tasks[j].name << ": " << std::fixed << std::setprecision(2) << wtime_task << std::endl;
                    }
                    // format: *.xx hours
                    std::cout << TERM_BOLD << "Total: " << std::fixed << std::setprecision(2) << wtime_proj << TERM_RESET << std::endl << std::endl;
                }
            }
        }else{
            std::cout << "report: Project list is empty." << std::endl;
        }
    }
}

// save project list to file, uses system("mkdir") to create diretory
// might be replaced by C++ filesystem API int the future
void command_save(ProjectList *proj_list){
    std::string file_name = tracking_dir+"/"+proj_list->month;
    std::string command = "mkdir -p "+tracking_dir;
    system (command.c_str());
    proj_list->save(file_name);
    std::cout << "Tracking data saved to file " << file_name << std::endl;
}

// discard current project list and load from file
void command_load(std::string date_str, ProjectList* proj_list){
    date_str = trim(date_str);
    std::string file_name = tracking_dir+"/"+date_str;

    // clear project list and load new one
    proj_list->projects.clear();
    proj_list->num_projects=0;
    proj_list->month=date_str;
    proj_list->load(file_name);

    // reset autocomplete
    autocomplete_names.clear();
    init_autocomplete(proj_list);
    std::cout << "Loaded project data for month " << proj_list->month << std::endl << std::endl;
}

// convert previous tracking file version to current one
void command_convert(std::string date_str){
    date_str = trim(date_str);
    ProjectList list(date_str);
    std::string file_name = tracking_dir+"/"+date_str;
    list.load(file_name);
    list.save(file_name);
    std::cout << "Successfully converted data file" << file_name << std::endl;
}

// print version number
void command_version(){
    std::cout << "tt v." << STRING(TT_VERSION) << std::endl;
    std::cout << "Author: Patrick Lipka (patrick.lipka@emea.nec.com)" << std::endl;
    std::cout << "tt is free software published under the GPLv3 licence, see http://www.gnu.org/licenses/gpl-3.0.html " << std::endl;
}
