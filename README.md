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
`USER_NAME="<Your full name>" make`
`sudo make install`

Note that there is not `/home` directory on macOS. You may want to set `TRACKING_DIR=/Users/$USER/track`.

## Settings


## Usage
