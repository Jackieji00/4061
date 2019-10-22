// Add Guard to header file (google what are guards)
// Function prototypes to
//			Traverse the Folder
//			Partition the text file paths to 'm' files
#ifndef PHASE1_H
#define PHASE1_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"

int partitionPharse(char * folderName,int numMapper);

#endif
