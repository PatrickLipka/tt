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
        int active_task;
};


#endif
