#ifndef TRACK_H
#define TRACK_H

#include "project.h"
extern int sigint;
extern int tracking;
void handler(int signum);
void track (Project *proj);

#endif
