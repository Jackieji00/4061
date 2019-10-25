// Add Guard to header file
// Function prototypes for
//			map function that will go through text files in the Mapper_i.txt to get the (letter, wordcount)
#ifndef PHASE2_H
#define PHASE2_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "variables.h"

void mapperPhase(char * folderName,int fd[2]);

#endif
