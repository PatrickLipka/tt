#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "project.h"

Project::Project(std::string name){
    Project::name = name;    
    Project::num_tasks = 0;
    Project::active_task_id = 0;
}

void Project::add_task(Task task){
    tasks.push_back(task);
    set_active_task(num_tasks);
    num_tasks++;
}

void Project::remove_task(int id){
    tasks.erase(tasks.begin()+id);
    set_active_task(std::max(id-1,0));
    num_tasks--;
}

void Project::set_active_task(int id){
    active_task_id = id;
    active_task = &tasks[id];
}

void ProjectList::add_project(Project proj){
    projects.push_back(proj);
    set_active_project(num_projects);
    num_projects++;
}

void ProjectList::remove_project(int id){
    projects.erase(projects.begin()+id);
    set_active_project(std::max(id-1,0));
    num_projects--;
}

void ProjectList::set_active_project(int id){
    active_project_id = id;
    active_project = &projects[id];
}


void ProjectList::save(std::string file_name){
    std::ofstream of(file_name, std::ios::binary);
    if (!of){
        std::cout << "Could not open file " << file_name << " for writing!" << std::endl;
        exit(1);
    }
    of.write((char*) &num_projects, sizeof(int));
    of.write((char*) &active_project_id, sizeof(int));
    for (int i=0; i<num_projects; i++){
        size_t len = projects[i].name.size();
        of.write((char*) &len, sizeof(size_t));
        of.write((char*) projects[i].name.c_str(),len);
        of.write((char*) &(projects[i].num_tasks), sizeof(int));
        of.write((char*) &(projects[i].active_task_id), sizeof(int));
        for (int j=0; j<projects[i].num_tasks; j++){
            size_t task_name_len = projects[i].tasks[j].name.size();
            of.write((char*) &task_name_len, sizeof(size_t));
            of.write((char*) projects[i].tasks[j].name.c_str(), task_name_len);
            of.write((char*) &(projects[i].tasks[j].work_time), sizeof(int));
        }
    }
    of.close();
}

void ProjectList::load(std::string file_name){
    std::ifstream inf(file_name, std::ios::binary);
    int active;
    if (!inf){
        std::cout << "Could not open file " << file_name << " for reading!" << std::endl;
        exit(1);
    }
    inf.read((char*) &num_projects,sizeof(int));
    inf.read((char*) &active, sizeof(int));
    int np = num_projects;
    int *active_task_arr = new int[np];
    for (int i=0; i<np;i++){
        size_t len;
        std::string proj_name;
        int active_t;
        inf.read((char*) &len, sizeof(size_t));
        char* tmp_name = new char[len+1];
        inf.read(tmp_name, len);
        tmp_name[len] = '\0';
        proj_name = tmp_name;
        Project proj(proj_name);
        delete [] tmp_name;
        inf.read((char*) &(proj.num_tasks), sizeof(int));
        inf.read((char*) &active_t, sizeof(int));
        int nt = proj.num_tasks;
        for (int j=0; j<nt; j++){
            std::string task_name;
            size_t task_name_len;
            inf.read((char*) &task_name_len, sizeof(size_t));
            char* tmp_task_name = new char[task_name_len+1];
            inf.read(tmp_task_name, task_name_len);
            tmp_task_name[len] = '\0';
            task_name = tmp_task_name;
            delete [] tmp_task_name;
            Task task(task_name);
            int work_time=0;
            inf.read((char*) &(work_time), sizeof(int));
            task.add_time(work_time);
            proj.add_task(task);   
        }
        // needed to get the active_task pointers right _after_ adding tasks to projetcs
        active_task_arr[i] = active_t;
        add_project(proj);
    }
    set_active_project(active);
    for (int i=0; i<np ; i++){
        projects[i].set_active_task(active_task_arr[i]);
    }
    delete [] active_task_arr;
    inf.close();
}
