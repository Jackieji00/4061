#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	if(line==0){return 1;}
	int l = strlen(line);
	int b =1;
//check by each character
	for(int i = 0;i<l;i++){
		if(isalnum(line[i])){
			b = 0;
		}
	}
	return b;
}

//check if there exists a comma
int hasComma(char line[]){
	if(line==0){return 0;}
	int l = strlen(line);
	int b =0;
//check by each character
	for(int i = 0;i<l;i++){
		if(line[i]==','){
			b = 1;
		}
	}
	return b;
}

//find the line number for the dep
//if return -1, then this dep doesn't exist
int findLine(char dep[]){
	int result = -1;
	if(isEmpty(dep)){return 0;}
	//iterate by lines
	for(int i = 0;strcmp(lines[i],"");i++){
		//check if there is dep name and : in this line
		if(strstr(lines[i],dep)!=NULL&&strstr(lines[i],":")!=NULL){
			//make sure the dep is in front of :
			if(strstr(lines[i],dep)-lines[i]<strstr(lines[i],":")-lines[i]){
				result = i;
			}
		}
	}
	return result;
}

// find the lase line number of recipes under that dep
int recipeCount(char dep[]){
	int depNum = findLine(dep)+1;
	int result = depNum;
	//iterate through lines
	for(int i = depNum;strcmp(lines[i],"");i++){
		if(strstr(lines[i],":")!=NULL||isEmpty(lines[i])){
			result = i;
		};
	}
	return result;
}
//count how many dependencies in the file
int depCount(){
	int result = 0;
	for(int i = 0;strcmp(lines[i],"");i++){
		if(strcmp(lines[i],":")>0){
			result++;
		}
	}
	return result;
}

//Validate the input arguments, bullet proof the program
int main(int argc, char *argv[]){
	//Error handleing
	if(argc==2){
		//takes 1 arguments
		if(!fopen(argv[1], "r")){
			fprintf(stderr,"%s doesn't exit\n", argv[1]);
			return 1;}
	}else if(argc==3){
		//takes 2 arguments
		if(strstr(argv[1],"-p")!=NULL){
			//with the flag -p
			if(!fopen(argv[2], "r")){
				fprintf(stderr,"%s doesn't exit\n", argv[2]);
				return 1;
			}
		}else if(strstr(argv[1],"-r")!=NULL){
			//with the flap -r
			if(!fopen(argv[2], "r")){
				fprintf(stderr,"%s doesn't exit\n", argv[2]);
				return 1;
			}
		}else if (strstr(argv[2],"-p")!=NULL||strstr(argv[2],"-p")!=NULL){
			//when 2nd argument is a flag
			fprintf(stderr,"Incorrect order\n");
			return 3;
		}else if(!fopen(argv[1], "r")){
			//when 1st argument is not vaild
			fprintf(stderr,"%s doesn't exit\n", argv[2]);
			return 1;
		}
	}else{
		//when arguments are more than 3.
		fprintf(stderr,"Too much arguements\n");
		return 2;
	}
	//$./mymake Makefile
	//Similarly account for -r flag
	if (argc == 2 && strncmp(argv[1], "-p", 2)) {
		process_file(argv[1]);
		//TODO
		// find numbers dependency
		char *** tokens = malloc(sizeof(char***));
		int numtokens = makeargv(lines[0],": \n", tokens);
		if(numtokens==1){
			//the case when there is no dependency
			// locate the recipe needed to run, by finding the start line and end line
			int recipe_start = findLine(tokens[0][0])+1;
			int recipe_last = recipeCount(tokens[0][0]);
			//iterate through each line for the recipe
			for (int i = recipe_start; i <= recipe_last; i++) {
				//check if this line has multiple recipes
				if(hasComma(lines[i])){
					//separate each recipes
					char *** recipeTokens = malloc(sizeof(char***));
					int numRecipe = makeargv(lines[i],"\t,\n", recipeTokens);
					//iterate each recipes in the same line
					for (int j = 0; j < numRecipe; j++) {
						pid_t pid = fork();
						if(pid==0){
							//remove space
							char *** args = malloc(sizeof(char***));
							int numargs = makeargv(recipeTokens[0][j],"\t \n", args);
							//put it into a char array
							char *arr[numargs+1];
							for (int k = 0; k < numargs; k++) {
								arr[k] = args[0][k];
							}
							// add NULL to end of array
							arr[numargs] = NULL;
							//run this recipe
							execvp(arr[0],arr);
						}else{
							wait(NULL);
						}
					}
				}else{
					//create a process per file
					pid_t pid = fork();
					if(pid ==0){
						//remove the newline and tab character
						char *** args = malloc(sizeof(char***));
						int numargs = makeargv(lines[i],"\t \n", args);
						//put it into a char array
						char *arr[numargs+1];
						for (int j = 0; j < numargs; j++) {
							arr[j] = args[0][j];
						}
						// add NULL to end of array
						arr[numargs] = NULL;
						//run this recipe
						execvp(arr[0],arr);
					}else{
						wait(NULL);
					}
				}
			}
		}else{
			//the case when target has multiple dependencies,
			//and deps needed to run their recipes
			int depNo[MAX_DEP];//record all the dependency
			int depIndex = 0;//keep reocrd array's index
			for (int i = numtokens-1; i >=0; i--) {
				char *startToken= tokens[0][i];
				//if the dep has recipe, find the line
				while(findLine(startToken)!=0){
					//record the line number
					depNo[depIndex] = findLine(startToken);
					//find the dep(s) for this target
					char *** tokensR= malloc(sizeof(char***));
					int n = makeargv(lines[depNo[depIndex]], ": \n", tokensR);
					int b = 1;
					//if there are multiple deps next to it,choose the one valid
					if(n>2){
						for (int z = 1; z < n; z++) {
							if(findLine(tokensR[0][z])!=0){
								b = z;
							}
						}
					}
					//update to the new dep needed to find
					startToken = tokensR[0][b];
					depIndex++;
				}
			}
			//run all the recipe for dep from the line record, and going back words
			for (int j = depIndex-1; j >=0; j--) {
				//iterate through all the recipe need to run under this dependency
				for (int k = 1;(strcmp(lines[depNo[j]+k],":")<=0)&&!isEmpty(lines[depNo[j]+k]); k++) {
					//check if this line has multiple recipes
					if(hasComma(lines[depNo[j]+k])){
						//separate each recipes
						char *** recipeTokens = malloc(sizeof(char***));
						int numRecipe = makeargv(lines[depNo[j]+k],"\t,\n", recipeTokens);
						//iterate each recipes in the same line
						for (int z = 0; z < numRecipe; z++) {
							pid_t pid = fork();
							if(pid==0){
								//remove space
								char *** args = malloc(sizeof(char***));
								int numargs = makeargv(recipeTokens[0][z],"\t \n", args);
								//put it into a char array
								char *arr[numargs+1];
								for (int x = 0; x < numargs; x++) {
									arr[x] = args[0][x];
								}
								// add NULL to end of array
								arr[numargs] = NULL;
								//run this recipe
								execvp(arr[0],arr);
							}else{
								wait(NULL);
							}
						}
					}else{
						pid_t pid = fork();
						if(pid ==0){
							//make the recipe into a char array with NULL ending
							char *** args = malloc(sizeof(char***));
							int numargs = makeargv(lines[depNo[j]+k],"\t \n", args);
							char *arr[numargs+1];
							for (int x = 0; x < numargs; x++) {
								arr[x] = args[0][x];
							}
							arr[numargs] = NULL;
							//run the recipe
							execvp(arr[0],arr);
						}else{
							wait(NULL);
						}
					}
				}
			}
			//run recipes for the target
			for (int i = 1;(strcmp(lines[i],":")<=0)&&!isEmpty(lines[i]); i++) {
				//check if this line has multiple recipes
				if(hasComma(lines[i])){
					//separate each recipes
					char *** recipeTokens = malloc(sizeof(char***));
					int numRecipe = makeargv(lines[i],"\t,\n", recipeTokens);
					//iterate each recipes in the same line
					for (int j = 0; j < numRecipe; j++) {
						pid_t pid = fork();
						if(pid==0){
							//remove space
							char *** args = malloc(sizeof(char***));
							int numargs = makeargv(recipeTokens[0][j],"\t \n", args);
							//put it into a char array
							char *arr[numargs+1];
							for (int k = 0; k < numargs; k++) {
								arr[k] = args[0][k];
							}
							// add NULL to end of array
							arr[numargs] = NULL;
							//run this recipe
							execvp(arr[0],arr);
						}else{
							wait(NULL);
						}
					}
				}else{
					pid_t pid = fork();
					if(pid ==0){
						//make the recipe into a char array with NULL ending
						char *** args = malloc(sizeof(char***));
						int numargs = makeargv(lines[i],"\t \n", args);
						char *arr[numargs+1];
						for (int j = 0; j < numargs; j++) {
							arr[j] = args[0][j];
						}
						arr[numargs] = NULL;
						//run the recipe
						execvp(arr[0],arr);
					}else{
						wait(NULL);
					}
				}
			}
		}
	}

	if (argc == 3) {
		// with flag -r
		if (!strncmp(argv[1], "-r",2)){
			if (!process_file(argv[2])){
				// find numbers dependency
				char *** tokens = malloc(sizeof(char***));
				int numtokens = makeargv(lines[0],": \n", tokens);
				if(depCount()==1){
					//if there is only one target/dependency in entire file
					//find the last line before next dependency
					int i,count = 0;
					for(i = 1;strcmp(lines[i],"");i++){
						if(!isEmpty(lines[i])){count++;} //count nonempty line
						if (strstr(lines[i],":")!=NULL){break;}//stop when it is dependency
					}
					int linNo[count];//record which is not empty slot
					int linNoCount =0;//as index for linNo
					//record all recipes' line number
					for (int j = 1; j < i; j++) {
						if(!isEmpty(lines[j])){
							linNo[linNoCount++] = j;
						}
					}
					//create an empty string to put all lines need to print
					int l = 0;
					for(int i = 0;i<count;i++){
						l += strlen(lines[linNo[i]]);

					}
					char* recipeLine = malloc(l+1);
					char *** tokenRecipe1= malloc(sizeof(char***));
					makeargv(lines[linNo[0]], "\t", tokenRecipe1);
					strcpy(recipeLine,lines[linNo[0]]);
					//add rest line to the string
					for(int k = 1;k<linNoCount;k++){
							strcat(recipeLine,lines[linNo[k]]);
					}
					//find tokens for recipes
					char *** tokensR= malloc(sizeof(char***));
					int numtokensR = makeargv(recipeLine, "\t\n,", tokensR);
					//print out recipes
					for (int j = 0; j < numtokensR; j++) {
						printf("%s\n",tokensR[0][j]);
					}
				}else{
					//iterate through dependencies of the target
					for (int i = 1; i < numtokens; i++) {
						int depNo[MAX_DEP];//record all the dependency needed to run
						int depIndex = 0;//index of depNo
						char *startToken= tokens[0][i];
						//if the dep has recipe, find the line of dep
						while(findLine(startToken)!=0){
							//record the line number
							depNo[depIndex] = findLine(startToken);
							//find the dep for this dependency
							char *** tokensR= malloc(sizeof(char***));
							int n = makeargv(lines[depNo[depIndex]], ": \n", tokensR);
							int b = 1;
							//if there are multiple statements next to it,find the one is valid
							if(n>2){
								for (int z = 1; z < n; z++) {
									if(findLine(tokensR[0][z])!=0){
										b = z;
									}
								}
							}
							//update to the new dep needed to find
							startToken = tokensR[0][b];
							depIndex++;

						}
						//print recipes in the array
						for (int j = depIndex-1; j >=0; j--) {
							for (int k = 1;(strcmp(lines[depNo[j]+k],":")<=0)&&!isEmpty(lines[depNo[j]+k]); k++) {
								char *** tokensRR= malloc(sizeof(char***));
								int numToken = makeargv(lines[depNo[j]+k], "\t,\n", tokensRR);
								for (int x = 0; x < numToken; x++) {
									printf("%s\n",tokensRR[0][x]);
								}
							}
						}
					}
					//print recipes for the target
					for (int i = 1;(strcmp(lines[i],":")<=0)&&!isEmpty(lines[i]); i++) {
						char *** tokensRR= malloc(sizeof(char***));
						int numToken = makeargv(lines[i], "\t,\n", tokensRR);
						for (int x = 0; x < numToken; x++) {
							printf("%s\n",tokensRR[0][x]);
						}
					}
				}
			}
		}else if (strncmp(argv[1], "-p", 2) && !process_file(argv[1])){
			//$./mymake Makefile target
			//TODO
			int depNo[MAX_DEP];//record all the dependency
			int depIndex = 0;//index of depNo
			//error handling if target doesn't exit
			char *** tokens = malloc(sizeof(char***));
			int numtokens = makeargv(lines[0],": \n", tokens);
			char *startToken= argv[2];
			if(findLine(startToken)==-1){
				fprintf(stderr,"%s doesn't exit\n", argv[2]);
				return 1;
			}
			//store whether the argv[2] is the target or not
			int boo = 0;
			//if this dep is target
			if(findLine(startToken)==0){
				boo =1;
				for (int i = numtokens-1; i>=0 ; i--) {
					char *startToken= tokens[0][i];
					//if the dep has recipe, find the line
					while(findLine(startToken)!=0){
						//record the line number
						depNo[depIndex] = findLine(startToken);
						//find the dep(s) for this target
						char *** tokensR= malloc(sizeof(char***));
						int n = makeargv(lines[depNo[depIndex]], ": \n", tokensR);
						int b = 1;
						//if there are multiple deps next to it,choose the one valid
						if(n>2){
							for (int z = 1; z < n; z++) {
								if(findLine(tokensR[0][z])!=0){
									b = z;
								}
							}
						}
						//update to the new dep needed to find
						startToken = tokensR[0][b];
						depIndex++;
					}
				}
			}else{
				//if the dep is not target and has recipe, find the line
				do{
					//record the line number
					depNo[depIndex] = findLine(startToken);
					//find the next dep for this target
					char *** tokensR= malloc(sizeof(char***));
					int n = makeargv(lines[depNo[depIndex]], ": \n", tokensR);
					int b = 1;//use to record which one of the dep is valid one
					//if there are multiple deps next to it, find the one valid one
					if(n>2){
						for (int z = 1; z < n; z++) {
							if(findLine(tokensR[0][z])!=0){
								b = z;
							}
						}
					}
						//update to the new dep needed to find
					startToken = tokensR[0][b];
					depIndex++;
				}while(findLine(startToken)!=0);
			}
				//run all the recipe for dep
			for (int j = depIndex-1; j >=0; j--) {
				//iterate through all the dependencies in the record
				for (int k = 1;(strcmp(lines[depNo[j]+k],":")<=0)&&!isEmpty(lines[depNo[j]+k]); k++) {
					//iterate through all the recipes under this dependency
					if (hasComma(lines[depNo[j]+k])) {
						//separate each recipes
						char *** recipeTokens = malloc(sizeof(char***));
						int numRecipe = makeargv(lines[depNo[j]+k],"\t,\n", recipeTokens);
						//iterate each recipes in the same line
						for (int x = 0; x < numRecipe; x++) {
							pid_t pid = fork();
							if(pid==0){
								//remove space
								char *** args = malloc(sizeof(char***));
								int numargs = makeargv(recipeTokens[0][x],"\t \n", args);
								//put it into a char array
								char *arr[numargs+1];
								for (int y = 0; y < numargs; y++) {
									arr[y] = args[0][y];
								}
								// add NULL to end of array
								arr[numargs] = NULL;
								//run this recipe
								execvp(arr[0],arr);
							}else{
								wait(NULL);
							}
						}
					}else{pid_t pid = fork();
						if(pid ==0){
							//make the string into a char array ending with NULL
							char *** args = malloc(sizeof(char***));
							int numargs = makeargv(lines[depNo[j]+k],"\t \n", args);
							char *arr[numargs+1];
							for (int i = 0; i < numargs; i++) {
								arr[i] = args[0][i];
							}
							arr[numargs] = NULL;
							//run the argument
							execvp(arr[0],arr);
						}else{
							wait(NULL);
						}
					}
				}
			}
			//run recipes for the target if the argv[2] is the target
			for (int i = 1;(strcmp(lines[i],":")<=0)&&!isEmpty(lines[i])&&boo==1; i++) {
				//iterate through all the recipes under the target
				pid_t pid = fork();
				if(pid ==0){
					//make the recipe into a char array ending with NULL
					char *** args = malloc(sizeof(char***));
					int numargs = makeargv(lines[i],"\t \n", args);
					char *arr[numargs+1];
					for (int j = 0; j < numargs; j++) {
						arr[j] = args[0][j];
					}
					arr[numargs] = NULL;
					//run the recipe;
					execvp(arr[0],arr);
				}else{
					wait(NULL);
				}
			}
		}
		if (!strncmp(argv[1], "-p", 2)) {
		//$./mymake -p Makefile
			if (!process_file(argv[2])) {
				//TODO
				int linRecord = 0;
				int totalline;
				for(totalline = 0;strcmp(lines[totalline],"");totalline++){
				}
				// for dependency
				do{
					char * dels = ": \n";
					char *** tokens = malloc(sizeof(char***));
			    int numtokens = makeargv(lines[linRecord], dels, tokens);
					int count=0;//count how many recipes under the target
					int linNo =1;
					for(int i = 1;strcmp(lines[linRecord+i],"");i++){
						linNo++;
						if(strstr(lines[linRecord+i],":")!=NULL||isEmpty(lines[i])){
							break;
						}
						if(hasComma(lines[linRecord+i])){
							char *** recipesToken = malloc(sizeof(char***));
					    int numRecipe = makeargv(lines[linRecord],",", recipesToken);
							count+= numRecipe;
						}else{count++;}
					}
					//print target info
					printf("target '%s' has %d dependencie(s) and %d recipe(s):\n",tokens[0][0],numtokens-1,count);
					//print out dependencies
					for (int i = 1; i < numtokens; i++) {
						printf("Dependency %d is %s\n",i-1,tokens[0][i]);
					}
					//concrete recipes to one string
					int l = 0;//store the string size
					for(int i = 1;i<linNo;i++){
						l += strlen(lines[i]);
					}
					char* recipeLine = malloc(l+1);
					//copy recipes to this new string
					strcpy(recipeLine,lines[linRecord+1]);
					for(int i = 2;i<linNo;i++){
						strcat(recipeLine,lines[linRecord+i]);
					}
					//find tokens for recipes
					char *** tokensR= malloc(sizeof(char***));
					makeargv(recipeLine, "\t,\n", tokensR);
					//print out recipes
					for (int j = 0; j < count; j++) {
						printf("Recipe %d is %s\n",j, tokensR[0][j]);
					}
					int k;
					for(k =linRecord+1;k<totalline;k++){
						if(strstr(lines[k],":")!=NULL){
							break;
						}
					}
					linRecord=k;
				}while(linRecord!=totalline);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
