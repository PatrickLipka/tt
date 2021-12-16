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
        Task *find_task_by_name(std::string task_name);
        int find_task_id_by_name(std::string task_name);
        int get_total_work_time();
};

class ProjectList{
    public:
        ProjectList(std::string month);
        std::string month;
        int num_projects;
        std::vector<Project> projects;
        void add_project(Project proj);
        void remove_project(int id);
        int active_project_id;
        Project *active_project;
        void set_active_project(int id);
        void save(std::string file_name);
        void load(std::string file_name, bool ignore_worktimes=false);
        Project *find_project_by_name(std::string proj_name);
        int find_project_id_by_name(std::string proj_name); 
};

#endif
