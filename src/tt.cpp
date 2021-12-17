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
#include "util.h"

// glovbal variables:
std::string user_name, tracking_dir, config_file;

int main(){ 
    // connect SIGINT signal (CTRL-C) to signal handler from track.h to use it to stop tracking of projects
    signal(SIGINT, handler);
    tracking = 0;

    // PREFIX set by preprocessor, see Makefile
    std::string prefix=STRING(PREFIX);
    config_file = prefix+"/etc/tt.conf";
    parse_config_file(config_file, &user_name, &tracking_dir);    
    
    // initialize project list for current month
    ProjectList proj_list(get_date());
    
    // if available: load project list for current month, else: start with list from last month
    std::string proj_file = tracking_dir+"/"+get_date();
    std::string last_proj_file = tracking_dir+"/"+get_last_date();
    std::ifstream f(proj_file);
    std::ifstream lf(last_proj_file);
    if(f){
        f.close();
        lf.close();
        proj_list.load(proj_file);
    }else if (lf){
        f.close();
        lf.close();
        // don't load work times from previous month
        proj_list.load(last_proj_file,true);
    }

    // add command names and names of project list to the autocomplete names
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
