#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "task.h"
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
    while (!sigint){
        sleep(1);
        worktime += 1;
    }
    proj->active_task->add_time(worktime);
    work_h = (worktime % 86400) / 3600;
    work_m = (worktime % 3600) / 60;
    work_s = worktime % 60;
    // TODO: replace with proper cout call
    printf("Time worked on project: %02d:%02d:%02d\n", work_h,work_m,work_s );
}

int main(){ 
    signal(SIGINT, handler);
    Task task("Test task");
    task.add_time(140);
    std::cout << "Task name: " << task.name << ", Work Time: " << task.work_time << std::endl;
    
    Project proj("Test Project");
    proj.add_task(task);
    std::cout << "Project name: " << proj.name << " No tasks:"<< proj.num_tasks <<" contains task 0: " << proj.tasks[0].name << " with work time " << proj.tasks[0].work_time << std::endl;
    
    /*std::cout << "Removing Task 0 from Project" << std::endl;
    proj.remove_task(0);
    std::cout << "Number of tasks in project: " << proj.num_tasks << std::endl;
    */
    ProjectList proj_list;
    proj_list.add_project(proj);
    std::cout << "Project List no of projects: " << proj_list.num_projects << " Name of active proj: " << proj_list.projects[proj_list.active_project_id].name << std::endl;

    std::cout << "Write Project List to file" << std::endl;
    std::cout << "Active Task: " << proj_list.active_project->active_task->name  << std::endl;
    proj_list.save("test.bin");
    
    std::cout << "Read Project List from file" << std::endl;
    ProjectList list2;
    list2.load("test.bin");
    
    std::cout << "Project List no of projects: " << list2.num_projects << std::endl;
    std::cout << "active Project: " << list2.active_project_id << std::endl;
    std::cout << "Name of active proj: " << list2.projects[list2.active_project_id].name << std::endl;
    std::cout << list2.active_project->name << std::endl;
    std::cout << list2.active_project->active_task->name << std::endl;
    std::cout << list2.active_project->active_task->work_time << std::endl;
    
    track(list2.active_project);

    return 0;
}
