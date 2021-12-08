#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "project.h"

Project::Project(std::string name){
    Project::name = name;    
    Project::num_tasks = 0;
    Project::active_task = 0;
}

void Project::add_task(Task task){
    tasks.push_back(task);
    active_task = num_tasks;
    num_tasks++;
}

void Project::remove_task(int id){
    tasks.erase(tasks.begin()+id);
    active_task = std::max(id-1,0);
    num_tasks--;
}

void ProjectList::add_project(Project proj){
    projects.push_back(proj);
    active_project = num_projects;
    num_projects++;
}

void ProjectList::remove_project(int id){
    projects.erase(projects.begin()+id);
    active_project = std::max(id-1,0);
    num_projects--;
}

void ProjectList::save(std::string file_name){
    std::ofstream of(file_name, std::ios::out | std::ios::binary);
    if (!of){
        std::cout << "Could not open file " << file_name << " for writing!" << std::endl;
        exit(1);
    }
    std::cout << num_projects << " " << num_projects << std::endl;
    of.write((char*) &num_projects, sizeof(int));
    of.write((char*) &active_project, sizeof(int));
    for (int i=0; i<num_projects; i++){
        int len = projects[i].name.length();
        of.write((char*) &len, sizeof(int));
        of.write((char*) &(projects[i].name),ProjectList::projects[i].name.length());
        of.write((char*) &(projects[i].num_tasks), sizeof(int));
        of.write((char*) &(projects[i].active_task), sizeof(int));
        for (int j=0; j<projects[i].num_tasks; j++){
            of.write((char*) &(projects[i].tasks[j].name), projects[i].tasks[j].name.length());
            of.write((char*) &(projects[i].tasks[j].work_time), sizeof(int));
        }
    }
    of.close();
}

void ProjectList::load(std::string file_name){
    std::ifstream inf(file_name, std::ios::in | std::ios::binary);
    if (!inf){
        std::cout << "Could not open file " << file_name << " for reading!" << std::endl;
        exit(1);
    }
    
    inf.read((char*) &num_projects,sizeof(int));
    std::cout << "Num projects read:" << num_projects << std::endl;
    inf.read((char*) &active_project, sizeof(int));
    for (int i=0; i<num_projects;i++){
        int tmp_len;
        std::string tmp_name;
        inf.read((char*) &tmp_len, sizeof(int));
        std::cout << "tmp_len read: "<< tmp_len <<std::endl;
        inf.read((char*) &tmp_name, tmp_len);
        std::cout << "name read: " << tmp_name << std::endl;
    }
}
