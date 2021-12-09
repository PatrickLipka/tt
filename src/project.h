#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include "task.h"

class Project{
    public:
        Project(std::string name);
        std::string name;
        int num_tasks;
        std::vector<Task> tasks;
        void add_task(Task task);
        void remove_task(int id);
        int active_task_id;
        Task *active_task;
        void set_active_task(int id);
};

class ProjectList{
    public:
        int num_projects;
        std::vector<Project> projects;
        void add_project(Project proj);
        void remove_project(int id);
        int active_project_id;
        Project *active_project;
        void set_active_project(int id);
        void save(std::string file_name);
        void load(std::string file_name);
};

#endif
