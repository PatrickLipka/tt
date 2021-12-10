#include <iostream>
#include <unistd.h>
#include "track.h"
#include "project.h"

int sigint;

void handler(int signum){
    sigint = 1;
    std::cout << std::endl;
}

void track(Project *proj){
    int worktime = 0;
    int work_h, work_m, work_s;
    time_t start;
    system("clear");
    time(&start);
    std::cout << "Started tracking of task " << proj->name << "/" << proj->active_task->name << " at " << ctime(&start) << std::endl;
    sigint = 0;
    while (!sigint){
        sleep(1);
        worktime += 1;
    }
    proj->active_task->add_time(worktime);
    work_h = (worktime % 86400) / 3600;
    work_m = (worktime % 3600) / 60;
    work_s = worktime % 60;
    
    printf("Time worked on project: %02d:%02d:%02d\n", work_h,work_m,work_s );
}
