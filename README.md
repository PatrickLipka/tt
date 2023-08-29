# tt
Simple command line time tracker for POSIX systems. \
Written by Patrick Lipka (patrick.lipka@gmail.com) \
tt is free software, distributed under the terms of the [GNU] General Public License as published by the Free Software Foundation, version 3 of the License. Please see LICENSE for details.

## Dependencies
The program uses GNU Readline (https://tiswww.case.edu/php/chet/readline/rltop.html) for command completion and history. \
The Readline library is free software, distributed under the terms of the [GNU] General Public License as published by the Free Software Foundation, version 3 of the License. \
Packages are available on most Linux distributions and BSD systems as well as macOS. Depending on your OS you may need to also install a package like libreadline-dev to have access to the headers.

The program assumes POSIX functionalities as provided by unistd.h and a Unix file system layout. The best way to run it on Windows might be to install a Unix compatibility layer or use Windows' Subsystem for Linux.\
To compile the program you need a C++ compiler which supports the C++11 standard.

The program has been tested on the following systems:
* Debian Linux 11.1, GCC 10.2.1
* Debian Linux 11.1, icpc (oneAPI) 2021.5.0
* CentOs Linux 7.7.1908, GCC 10.2.0
* Manjaro Linux 21.1, GCC 11.1.0
* macOS 11.5.2, Apple clang 12.0.5

## Installation

### Makefile variables
The following variables can be used to configure the build:
* `CXX` - C++ compiler, defaults to `g++`
* `PREFIX` - install prefix, defaults to `/usr/local`
* `USER_NAME` - user name to be mentioned in monthly reports, defaults to `$USER`
* `TRACKING_DIR` - directory where tracking files are saved, defaults to `$HOME/track`
* `CPPFLAGS` - set to `-UFORMATTED_TEXT` if your terminal does not support usual Unix escape sequences

The user name and tracking directory can also be set after installation by modifying `$PREFIX/etc/tt.conf`

### Build command
The default install process is:
~~~~
USER_NAME="<Your full name>" make
sudo make install
~~~~

## Settings
The file `$PREFIX/etc/tt.conf` has the format `key=value` and contains the following settings:
* `user_name` - user name to be mentioned in monthly reports
* `tracking_directory` - directory where tracking files are saved and loaded from

## Usage
The program `tt` does not take any command line arguments. Once launched, it provides a command prompt which supports TAB completion and a command history using the arrow keys. Note that white spaces are displayed as underscores in completion suggestions.\
The program can be exited by pressing CTRL-C (catches SIGINT).

At startup, the config file `$PREFIX/etc/tt.conf` is beeing read and tracking data is loaded from `tracking_directory` if available. In case there is no tracking data available for the current month, the project structures are loaded from the previous month's file. In case there is no data available, an empty project list is beeing set up.

The general concept is to have a list of projects with unique names. Each project has its own list of tasks which can be set active and for which time can be tracked.\
One task cannot be part of multiple projects. Tasks are not further divided into subtasks.

The following commands are available:
#### `ls [<Project Name>]`
Prints active project, active task (if present) and the names of all projects.\
If the optional parameter `<Project Name>` is present, a list of Tasks for this project is printed.

Example:
~~~
tt> ls
Active Project/Task: HPCE/Aurora Compiler Testsuite

List of Projects:
HPCE
~~~
~~~
tt> ls HPCE
List of Tasks for Project HPCE:
Various
Time Tracker Development
~~~

#### `np <Project Name>`
Creates a new empty project with the given name and sets it active. The name must be different from the existing projects.

Example:
~~~
tt> np Test Project
Switched to project Test Project
~~~

#### `nt [<Project Name>/]<Task Name>`
Creates a new task with the given name inside the currently active project and sets it active.\
If the optional parameter `<Project Name>` is present, the task is beeing created in the specified Project instead. This also changes the active project to `<Project Name>`\
The task name must be different from existing tasks.

Examples:
~~~
tt> nt Test Task
Switched to task Test Project/Test Task
~~~
~~~
tt> nt HPCE/test
Switched to task HPCE/test
~~~

#### `rm [<Project Name>/]<Task Name> | <Project Name>`
Deletes all data of a project or task.\ 
If only a project name is provided, the project with all tasks is beeing deleted. The previous project in the project list is set active\
If only a task name is provided, this task is beeing deleted from the active project if present. The previous task in the task list is set active.\
If `<Project Name>/<Task Name>` is provided, the task with name `<Task Name>` is deleted from project `<Project Name>` if both are present.

Examples:
~~~
tt> rm test
Do you really want to remove task HPCE/test? [y|n]
y
Removed  task HPCE/test
Switched to task HPCE/Time Tracker Development
Tracking data saved to file /home/patrick/track/2021-12
~~~
~~~
tt> rm Test_Project/Test_Task 
Do you really want to remove task Test Project/Test Task? [y|n]
y
Removed  task Test Project/Test Task
Tracking data saved to file /home/patrick/track/2021-1
~~~
~~~
tt> rm Test_Project 
Do you really want to remove project Test Project? [y|n]
y
Removed project Test Project.
Switched to project HPCE
Tracking data saved to file /home/patrick/track/2021-12
~~~

#### `re [<Project Name>/]<Task Name> <New Task Name> |<Project Name> <New Project Name>`
Renames tasks or projects to the new name.\
If only a project name is provided, the project is beeing renamed. \
If only a task name is provided, this task inside the active project is renamed. \
If `<Project Name>/<Task Name>` is provided, the task with name `<Task Name>` in Project `<Project Name>` is renamed.

Examples:
~~~
tt> re Tets Test
Renamed task Test Project/Tets to Test Project/Test
Tracking data saved to file /home/patrick/track/2021-12
~~~
~~~
tt> re Test_Project/Test_Task Another Task
Renamed task Test Project/Test Task to Test Project/Another Task
Tracking data saved to file /home/patrick/track/2021-12
~~~
~~~
tt> re Tets_Projekt Test Project
Renamed  project Tets Projekt to Test Project
Tracking data saved to file /home/patrick/track/2021-12
~~~

#### `sp <Project Name>`
Switches to another project, thus setting Project with name `<Project Name>` active.

Example:
~~~
tt> sp Project 2
Switched to project Project 2
~~~

#### `st [<Project Name>/]<Task Name>`
Switches to another task of the same project.\
If the optional parameter `<Project Name>` is present, the task with name `<Task Name>` of project `<Project Name>` is set active. This also causes the active project to change to `<Project Name>`.

Examples:
~~~
tt> st Task 2
Switched to task Project 2/Task 2
~~~
~~~
tt> st Test_Project/Debugging 
Switched to task Test Project/Debugging
~~~

#### `start <Project Name>/][<Task Name>]`
Starts tracking for the currently active task.\
If the optional parameter `<Project Name>` is present, the task with name `<Task Name>` of project `<Project Name>` is set active and tracking starts. This also causes the active project to change to `<Project Name>`.\
Tracking can be stopped by pressing `CTRL-C`.

Examples:
~~~
tt> start
Started tracking of task Test Project/Debugging at Fri Dec 17 16:07:43 2021

Press CTRL-C to stop tracking
^C
Time worked on project: 00:00:02
Tracking data saved to file /home/patrick/track/2021-12
~~~
~~~
start Project_2/Task_1
Started tracking of task Project 2/Task 1 at Fri Dec 17 16:10:07 2021

Press CTRL-C to stop tracking
^C
Time worked on project: 00:00:06
Tracking data saved to file /home/patrick/track/2021-12
~~~

#### `at <time>[s/m/h] [<Project Name>/][<Task Name>]`
Manually adds `<time>` to task's work time. If `<time>` is followed by `m` or `h`, `<time>` is interpreted as minutes / hours. With `s` or without a postfix, time is interpreted as seconds. \
If the optional parameter `<Task Name>` is present, the time is beeing added to the Task with name `<Task Name>` of the currently active project.\
If also the optional parameter `<Project Name>` is present, the time is beeing added to the task with name `<Task Name>` of project `<Project Name>`.

Examples:
~~~
tt> at 30
30s added to task Test Project/Debugging
Tracking data saved to file /home/patrick/track/2021-12
~~~
~~~
tt> at 2h Task 2
7200s added to task Test Project/Task 2
Tracking data saved to file /home/patrick/track/2022-01
~~~
~~~
tt> at 120s Project_2/Task_1
120s added to task Project 2/Task 
Tracking data saved to file /home/patrick/track/2021-12
~~~
#### `rt [<Project Name>/]<Task Name>`
Remove time from task. Usage identical to `at`.

#### `report [<yyyy-mm>]`
Generate report for current month. This does only list tasks with working time of more than 0.01h in current month. \
If the optional argument `<yyyy-mm>` is present, a report for month `mm` of year `yyyy` is beeing generated from file `<tracking_directory>/yyyy-mm`.

Examples:
~~~
tt>report
Report for Patrick Lipka, month: 2021-12

Project: Test Project
--- Task 2: 0.04
Total: 0.05

Project: Project 2
--- Task 1: 0.03
--- Task 2: 16.61
Total: 16.65
~~~
~~~
tt> report 2019-05
Report for Patrick Lipka, month: 2019-05

Project: Client 1
--- Support: 1.67
Total: 1.67

Project: Code Project
--- Implementation: 15.09
--- Debugging: 9.26
Total: 24.35
~~~

### `save`
Saves the project list to file `tracking_dir/yyy-mm`.\
This is automatically done after each meaningful change to the project list.

### `load <yyyy-mm>`
Discards the currently loaded project list and loads data from a tracking file instead.

### `convert <yyyy-mm>`
Converts tracking files written by previous versions of tt to the current format.

#### `version`
Prints the version number of `tt`
