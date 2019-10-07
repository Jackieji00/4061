#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

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
// makeargv from the book
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
//check if the string is empty or not
int isEmpty(char line[]){
	int l = strlen(line);
	int b =1;
	for(int i = 0;i<l;i++){
		if(isalnum(line[i])){
			b = 0;
		}
	}
	return b;
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
		// with flag -r
		if (!strncmp(argv[1], "-r", 2)){
			if (!process_file(argv[2])){
				//find the last line of dependency
				int count=0;
				for(int i = 0;strcmp(lines[i],"");i++){
					if(!(strcmp(lines[i],":")>0||isEmpty(lines[i]))){
						count++;
						// printf("%d: %s\n",i,lines[i]);
					}
					// printf("%d: %s\n",i,lines[i] );
				}
				// printf("%d\n",count );
				int linNo[count];//record which lines need to print
				int linNoCount =0;
				for(int i = 0;strcmp(lines[i],"");i++){
					if(!(strcmp(lines[i],":")>0||isEmpty(lines[i]))){
						linNo[linNoCount] = i;
						linNoCount++;
						// printf("%d: %s\n",i,lines[i]);
					}
				}
				//create an empty string to put all lines need to print
				int l = 0;
				for(int i = 0;i<count;i++){
					l += strlen(lines[linNo[i]]);
				}
				char* recipeLine = malloc(l+1);
				//concrete recipes to one string
				strcpy(recipeLine,lines[linNo[count-1]]);
				for(int i = count-2;i>=0;i--){
					strcat(recipeLine,lines[linNo[i]]);
				}
				//find tokens for recipes
				char *** tokensR= malloc(sizeof(char***));
				makeargv(recipeLine, "\t\n", tokensR);
				//print out recipes
				for (int j = 0; j < linNoCount; j++) {
					printf("%s\n",tokensR[0][j]);
				}
			}
		}
		//$./mymake Makefile target
		else if (strncmp(argv[1], "-p", 2) && !process_file(argv[1])) {
			//TODO
			printf("%s\n","2" );
		} else if (!strncmp(argv[1], "-p", 2)) {
			//$./mymake -p Makefile
			if (!process_file(argv[2])) {
				//TODO

				// for dependency
				char * dels = ": ";
				char *** tokens = malloc(sizeof(char***));
		    int numtokens = makeargv(lines[0], dels, tokens);

				//find linenumber for recipe
				int count;
				for(count = 1;strcmp(lines[count],"");count++){
					if(strcmp(lines[count],":")>0||isEmpty(lines[count])){
						break;
					}
					printf("%d: %s\n",count,lines[count] );
				}

				//print target info
				printf("target '%s' has %d dependencie(s) and %d recipe(s):\n",tokens[0][0],numtokens-1,count-1);
				//print out dependencies
				for (int i = 1; i < numtokens; i++) {
					printf("Dependency %d is %s\n",i-1,tokens[0][i]);
				}
				//concrete recipes to one string
				int l = 0;
				for(int i = 1;i<count;i++){
					l += strlen(lines[i]);
				}
				char* recipeLine = malloc(l+1);
				strcpy(recipeLine,lines[1]);
				for(int i = 2;i<count;i++){
					strcat(recipeLine,lines[i]);
				}
				//find tokens for recipes
				char *** tokensR= malloc(sizeof(char***));
				makeargv(recipeLine, "\t\n", tokensR);
				//print out recipes
				for (int j = 1; j < count; j++) {
					printf("Recipe %d is %s\n",j-1, tokensR[0][j-1]);
				}
			}
		}



	}

	exit(EXIT_SUCCESS);
}
