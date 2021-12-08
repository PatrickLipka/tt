#include <vector>
#include <algorithm>
#include "project.h"

Project::Project(std::string name){
    Project::name = name;    
    Project::num_tasks = 0;
    Project::active_task = 0;
}

void Project::add_task(Task task){
    Project::tasks.push_back(task);
    Project::active_task = Project::num_tasks;
    Project::num_tasks++;
}

void Project::remove_task(int id){
    Project::tasks.erase(tasks.begin()+id);
    Project::active_task = std::max(id-1,0);
    Project::num_tasks--;
}

void ProjectList::add_project(Project proj){
    ProjectList::projects.push_back(proj);
    ProjectList::active_project = ProjectList::num_projects;
    ProjectList::num_projects++;
}

void ProjectList::remove_project(int id){
    ProjectList::projects.erase(projects.begin()+id);
    ProjectList::active_project = std::max(id-1,0);
    ProjectList::num_projects--;
}
