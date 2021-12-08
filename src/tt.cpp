#include <iostream>
#include <vector>
#include "task.h"
#include "project.h"

int main(){
    
    Task task("Test task");
    task.add_time(140);
    std::cout << "Task name: " << task.name << ", Work Time: " << task.work_time << std::endl;
    
    Project proj("Test Project");
    proj.add_task(task);
    std::cout << "Project name: " << proj.name << " No tasks:"<< proj.num_tasks <<" contains task 0: " << proj.tasks[0].name << " with work time " << proj.tasks[0].work_time << std::endl;
    
    std::cout << "Removing Task 0 from Project" << std::endl;
    proj.remove_task(0);
    std::cout << "Number of tasks in project: " << proj.num_tasks << std::endl;
    
    ProjectList proj_list;
    proj_list.add_project(proj);
    std::cout << "Project List no of projects: " << proj_list.num_projects << " Name of active proj: " << proj_list.projects[proj_list.active_project].name << std::endl;

    std::cout << "Write Project List to file" << std::endl;
    proj_list.save("test.bin");
    
    std::cout << "Read Project List from file" << std::endl;
    ProjectList list2;
    list2.load("test.bin");

    return 0;
}
