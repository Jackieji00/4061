#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "dfs_stack.h"

//Parse the input makefile to determine targets, dependencies, and recipes
int process_file(char *fname)
{
	FILE* fp = fopen(fname, "r");
	char line[LINE_SIZE];
	int i = 0;

	if (!fp) {
		printf("Failed to open the file: %s \n", fname);
		return -1;
	}

	//Read the contents and store in lines
	while (fgets(line, LINE_SIZE, fp)) 
		strncpy(lines[i++], line, strlen(line));

	fclose(fp);

	return 0;
}

//Validate the input arguments, bullet proof the program
int main(int argc, char *argv[]) 
{
	//$./mymake Makefile
	//Similarly account for -r flag
	if (argc == 2 && strncmp(argv[1], "-p", 2)) {
		process_file(argv[1]); 
		//TODO
	}

	if (argc == 3) {
		//$./mymake Makefile target
		if (strncmp(argv[1], "-p", 2) && !process_file(argv[1])) {
			//TODO
			
		} else if (!strncmp(argv[1], "-p", 2)) {
			//$./mymake -p Makefile 
			if (!process_file(argv[2])) {
				//TODO
			}
		}
	}

	exit(EXIT_SUCCESS);
}
