#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "task.h"
#include "project.h"
#include "track.h"
#include "ui.h"
#include "tt.h"

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

std::string user_name, tracking_dir, config_file;



int main(){ 
    // connect SIGINT signal (CTRL-C) to signal handler from track.h to use it to stop tracking of projects
    signal(SIGINT, handler);
    tracking = 0;
    std::string prefix=STRING(PREFIX);
    config_file = prefix+"/etc/tt.conf";
    parse_config_file(config_file, &user_name, &tracking_dir);    
    
    // TEST: set up test list
    ProjectList proj_list("dec");
    Project proj("Test Project");
    Task task1("Task 1"),task2("Task 2"),task3("Task 3");
    proj.add_task(task1);
    proj.add_task(task2);
    proj.add_task(task3);
    proj_list.add_project(proj);
   // proj_list.load(get_date());

    init_autocomplete(&proj_list);
    // use GNU readline for auto completion and history when parsing command input
    while(1) {

        rl_attempted_completion_function = tt_name_completion;
        char *buffer = readline("tt> ");
        if (buffer){
            std::string str = buffer;
            parse_input(str,&proj_list);
            if(str.length() > 1) add_history(buffer);
            free(buffer);
        }
    }
    return 0;
}
