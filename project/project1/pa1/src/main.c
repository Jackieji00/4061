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
	// printf("isEmpty: %s\n",line );
	if(line==0){return 1;}
	int l = strlen(line);
	int b =1;

	for(int i = 0;i<l;i++){
		if(isalnum(line[i])){
			b = 0;
		}
	}
	return b;
}
//find the index of a string contains in a string
// int indexof(char dep[],char line[]){
//
// }
//find the line number for the dep
int findLine(char dep[]){
	int result = 0;

	if(isEmpty(dep)){return 0;}
	// printf("%s\n","h" );
	for(int i = 1;strcmp(lines[i],"");i++){
		// printf("%ld\n",strstr(lines[i],dep) );
		if(strstr(lines[i],dep)!=NULL&&strstr(lines[i],":")!=NULL){
			// printf("%ld,%ld\n",strstr(lines[i],dep)-lines[i],strstr(lines[i],":")-lines[i] );
			if(strstr(lines[i],dep)-lines[i]<strstr(lines[i],":")-lines[i]){
				result = i;
				// printf("line:%d\n",result );
			}
		}

	}
	return result;
}
// find the line number that end of recipe undet that dep
int recipeCount(char dep[]){
	int depNum=findLine(dep);
	int result = 0;
	for(int i = depNum;strcmp(lines[i],"");i++){
		if(strcmp(lines[i],":")>0||isEmpty(lines[i])){
			result = i;
		};
	}
	return result;
}
int depCount(){
	int result = 0;
	for(int i = 0;strcmp(lines[i],"");i++){
		if(strcmp(lines[i],":")>0){
			// printf("%d:%s\n",i,lines[i] );
			result++;
		}
	}
	return result;
}
//Validate the input arguments, bullet proof the program
int main(int argc, char *argv[])
{
	//$./mymake Makefile
	//Similarly account for -r flag
	if (argc == 2 && strncmp(argv[1], "-p", 2)) {
		process_file(argv[1]);
		//TODO
		// find numbers dependency
		// char *** tokens = malloc(sizeof(char***));
		// int numtokens = makeargv(lines[0],": ", tokens);
		// if(numtokens==1){
		// 	//run all recipe
		// 	// locate the recipe need to run
		// 	int recipe_start = findLine(tokens[0][0])+1;
		// 	int recipe_last = recipeCount(tokens[0][0]);
		// 	for (int i = recipe_start; i < recipe_last; i++) {
		// 		//create a process per line
		// 		pid_t pid = fork();
		// 		if(pid ==0){
		// 			execv(lines[i]);
		// 		}else{
		// 			wait(Null);
		// 		}
		// 	}
		// }else{
		// 	//run required recipes
		//
		// }
	}

	if (argc == 3) {
		// with flag -r
		if (!strncmp(argv[1], "-r", 2)){
			if (!process_file(argv[2])){
				// find numbers dependency
				char *** tokens = malloc(sizeof(char***));
				int numtokens = makeargv(lines[0],": \n", tokens);
				if(depCount()==1){
					//find the last line before next dependency
					int i,count = 0;
					for(i = 1;strcmp(lines[i],"");i++){
						if(!isEmpty(lines[i])){count++;} //count nonempty line
						if (strcmp(lines[i],":")){break;}
					}
					int linNo[count];//record which is not empty slot
					int linNoCount =0;//as index for linNo
					for (int j = 0; j < i; j++) {
						if(!isEmpty(lines[i])){
							linNo[linNoCount++] = i;
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
				}else{
					for (int i = 1; i < numtokens; i++) {
						int depNo[MAX_DEP];//record all the dependency
						int depcount = 0;
						char *startToken= tokens[0][i];
						//if the dep has recipe, find the line
						while(findLine(startToken)!=0){
							//record the line number
							depNo[depcount] = findLine(startToken);
							//find the dep for this target
							char *** tokensR= malloc(sizeof(char***));
							int n = makeargv(lines[depNo[depcount]], ": \n", tokensR);
							int b = 1;
							//if there are multiple statements next to it
							if(n>2){
								for (int z = 1; z < n; z++) {
									if(findLine(tokensR[0][z])!=0){
										b = z;
									}
								}
							}
							//update to the new dep needed to find
							startToken = tokensR[0][b];
							depcount++;

						}
						//print recipes in the array
						for (int j = depcount-1; j >=0; j--) {
							for (int k = 1;(strcmp(lines[depNo[j]+k],":")<=0)&&!isEmpty(lines[depNo[j]+k]); k++) {
								char *** tokensRR= malloc(sizeof(char***));
								makeargv(lines[depNo[j]+k], "\t\n", tokensRR);
								printf("%s\n",tokensRR[0][0]);
							}
						}
					}
					//print recipes for the target
					for (int i = 1;(strcmp(lines[i],":")<=0)&&!isEmpty(lines[i]); i++) {
						char *** tokensRR= malloc(sizeof(char***));
						makeargv(lines[i], "\t\n", tokensRR);
						printf("%s\n",tokensRR[0][0]);
					}
				}
			}
		}
		//$./mymake Makefile target
		if (strncmp(argv[1], "-p", 2) && !process_file(argv[1])) {
			//TODO


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
