// Add Guard to header file
// Function prototype for
//			reduce function that will receive list from all the Mappers to create a single list
//			the single list is then written to ReducerResult.txt
#ifndef PHASE3_H
#define PHASE3_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"

void reduceResult(int ** fd, int numOfMapper);

#endif
