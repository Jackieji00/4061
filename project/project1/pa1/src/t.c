#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int makeargv(const char *s, const char *delimiters, char ***argvp) {
    int error;
    int i;
    int numtokens;
    const char *snew;
    char *t;

    if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
        errno = EINVAL;
        return -1;
    }
    *argvp = NULL;
    snew = s + strspn(s, delimiters);
    if ((t = malloc(strlen(snew) + 1)) == NULL)
        return -1;
    strcpy(t, snew);
    numtokens = 0;
    if (strtok(t, delimiters) != NULL)
        for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++);

    if ((*argvp = malloc((numtokens + 1) * sizeof(char *))) == NULL) {
        error = errno;
        free(t);
        errno = error;
        return -1;
    }

    if (numtokens == 0)
        free(t);
    else {
        strcpy(t, snew);
        **argvp = strtok(t, delimiters);
        for (i = 1; i < numtokens; i++)
            *((*argvp) + i) = strtok(NULL, delimiters);
    }
    *((*argvp) + numtokens) = NULL;
    return numtokens;
}

//Validate the input arguments, bullet proof the program
int main(int argc, char *argv[])
{
	//$./mymake Makefile
	//Similarly account for -r flag
	if (argc == 2 && strncmp(argv[1], "-p", 2)) {
		process_file(argv[1]);
		//TODO
		int toknum = 0;
		//    char * line = "---abcabc-efefef-ggg---";
   // char * dels = "-";
    // char * line = "t1: d1 d2 d3";
    // char * dels = ": ";
   // char * line = "\tr1\nr2";
   // char * dels = "\t\n";

    char *** tokens = malloc(sizeof(char***));
		char ** lines = malloc(sizeof(char**));
		lines[0] = "t1: d1 d2 d3";
		lines[1] = "\tr1\br2\n";
		lines[2] = "\tr3\br4\n";
		char * dels ="\t\n";
		int count = 0;
		for(int i = 0;lines[i]!=NULL;i++){count++;}
		printf("%d\n",count );
    int numtokens = makeargv(lines[0], dels, tokens);
    printf("Num tokens : %d\n", numtokens );
    printf("First token : %s\n", tokens[0][0]);
    printf("Second token : %s\n", tokens[0][1]);

		// char* tok = strtok(lines," ");
		// for(toknum = 1;strtok(NULL," ")!=NULL;toknum++);
		// printf("tok:%s\n", tok);

		// printf("target '%s' has %d dependencie(s) and %d recipe(s)\n", );
		// //print out dependencies
		// for (int i = 0; i < count; i++) {
		// 	printf("Dependency %d is %s\n",i, );
		// }
		// //print out recipes
		// for (int j = 0; j < count; j++) {
		// 	printf("Recipe %d is %s\n",j, );
		// }
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
