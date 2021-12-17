# tt
Simple command line time tracker for POSIX systems. \
Written by Patrick Lipka (patrick.lipka@emea.nec.com) \
This software is licensed under the BSD-2-clause license. Please see LICENSE for details.

## Dependencies
The program uses GNU Readline (https://tiswww.case.edu/php/chet/readline/rltop.html) for command completion and history. \
The Readline library is free software, distributed under the terms of the [GNU] General Public License as published by the Free Software Foundation, version 3 of the License. \
Packages are available on most Linux distributions and BSD systems as well as macOS. Depending on your OS you man need to also install a package like libreadline-dev to have access to the headers.

The program assumes POSIX functionalities as provided by unistd.h and a Unix file system layout. The best way to run it on Windows might be to install a Unix compatibility layer or use Windows' Subsystem for Linux.\
To compile the program you need a C++ compiler which supports the C++11 standard.

The program has been tested on the following systems:
* Debian Linux 5.10.70-1, GCC 10.2.1 20210110
* macOS 11.5.2, Apple clang 12.0.5

## Installation

### Makefile variables
The following variables can be used to configure the build:
* `CXX` - C++ compiler, defaults to `g++`
* `PREFIX` - install prefix, defaults to `/usr/local`
* `USER_NAME` - user name to be mentioned in monthly reports, defaults to `$USER`
* `TRACKING_DIR` - directory where tracking files are saved, defaults to `/home/$USER/track`

The user name and tracking directory can also be set after installation by modifying `$PREFIX/etc/tt.conf`

### Build command
The default install process is:
~~~~
USER_NAME="<Your full name>" make
sudo make install
~~~~

Note that there is no `/home` directory on macOS. You may want to set `TRACKING_DIR=/Users/$USER/track`.

## Settings
The file `$PREFIX/etc/tt.conf` has the format `key=value` and contains the following settings:
* `user_name` - user name to be mentioned in monthly reports
* `tracking_directory` - directory where tracking files are saved and loaded from

## Usage
The program `tt` does not take any command line arguments. Once launched, it provides a command prompt which supports TAB completion and a command history using the arrow keys. It can be exited by pressing CTRL-C (catches SIGINT).

At startup, the config file `$PREFIX/etc/tt.conf` is beeing read and tracking data is loaded from `tracking_directory` if available. In case there is no tracking data available for the current month, the project structures are loaded from the previous month's file. In case there is no data available, an empty project list is beeing set up.

The general concept is to have a list of projects with unique names. Each project has its own list of tasks which can be set active and for which time can be tracked.\
One task cannot be part of multiple projects. Tasks are not further divided into subtasks.

The following commands are available:
#### `ls [<Project Name>]`
Prints active project, active task (if present) and the names of all projects.\
If the optional parameter `<Project Name>` is present, a list of Tasks for this project is printed.\
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
Creates a new empty project with the given name and sets it active. The name must be different from the existing projects.\
Example:
~~~
tt> np Test Project
Switched to project Test Project
~~~

#### `nt [<Project Name>/]<Task Name>`
Creates a new task with the given name inside the currently active project and sets it active.\
If the optional parameter `[<Project Name>]` is present, the task si beeing created in the specified Project instead. This also changes the active project\
The task name must be different from existing tasks.\
Example:
~~~
tt> nt Test Task
Switched to task Test Project/Test Task
~~~
~~~
tt> nt HPCE/test
Switched to task HPCE/test
~~~

#### `rm [<Project Name>/]<Task Name>`


#### `re [<Project Name>/]<Task Name>`

#### `sp <Project Name>`

#### `st [<Project Name>/]<Task Name>`

#### `start <Project Name>/]<Task Name>`

#### `at [<Project Name>/]<Task Name>`

#### `rt [<Project Name>/]<Task Name>`

#### `report [<yyyy-mm>]`
