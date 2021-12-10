#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "task.h"
#include "project.h"
#include "track.h"

int main(){ 
    signal(SIGINT, handler);

    return 0;
}
