#include <iostream>
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
    return 0;
}
