#include <iostream>
#include <algorithm>
#include "task.h"

Task::Task(std::string name){
    Task::name = name;
    Task::work_time = 0;
}

void Task::add_time(int seconds){
    work_time = std::max(work_time+seconds,0); 
}
