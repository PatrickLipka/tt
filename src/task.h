#ifndef TASK_H
#define TASK_H

#include <string>
#include <algorithm>

class Task{
    public:
        Task(std::string name);
        std::string name;
        int work_time;
        void add_time(int seconds);
};

#endif
