#include "main.h"

//check if it number
int isNum(char * argv2){
	int result = 1;
	for (int i = 0; i < strlen(argv2); i++) {
		if(!isdigit(argv2[i])){
		result = 0;
		}
	}
	return result;
}

int main(int argc, char *argv[]){
	// check if arguements are right
	if(argc<3){
		fprintf(stderr," Need more argument\n");
		return 1;
	}else if(argc==3){
		if(!isNum(argv[2])){
			fprintf(stderr," %s is not a number\n",argv[2]);
			return 2;
		}
		DIR *dr;
		if((dr=opendir(argv[1]))==NULL){
			fprintf(stderr, "%s is invalid\n",argv[1] );
			return 2;
		}
	}else{
		fprintf(stderr,"Too much arguements\n");
		return 3;
	}
	//just make a function call to code in phase1.c
	//phase1 - Data Partition Phase
	int numMapper = atoi(argv[2]);
	partitionPharse(argv[1],numMapper);

	if(fileCount==0){
		fprintf(stderr,"The directory is empty\n");
		return 4;
	}
	//create pipes and initialize the pipe
	int fd[numMapper][2];
	pid_t pids[numMapper];
	for (int i = 0; i < numMapper; i++) {
		if(pipe(fd[i])==-1){
			fprintf(stderr,"Fail to pipe\n");
			return 4;
		}
	}
	//just make a function call to code in phase2.c
	//phase2 - Map Function
	//let the child processes do the mapper phase.
	char * dicName =  malloc(SIZE_TXTPATH*sizeof(char));;
	for (int i = 0; i < numMapper; i++) {
			pids[i]=fork();
			if (pids[i]<0) {
				fprintf(stderr,"Fail to folk\n");
				return 4;
			}else if(pids[i]==0){
				sprintf(dicName,"MapperInput/Mapper_%d.txt",i);
				mapperPhase(dicName,fd[i]);
				free(dicName);
				_exit(1);
			}
	}

	//just make a function call to code in phase3.c
	//phase3 - Reduce Function
	reduceResult(fd, numMapper);
	//phase4
	//wait for all processes to reach this point
	//just make a function call to code in phase4.c
	//master process reports the final output
	finalResult();
	return 0;
}
