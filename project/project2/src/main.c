#include "main.h"


int isNum(char * argv2){
	int result = 1;
	for (int i = 0; i < strlen(argv2); i++) {
		/* code */
		if(!isdigit(argv2[i])){
		result = 0;
		}
	}
	return result;
}

int main(int argc, char *argv[]){
	DIR *dr;
	// argument count check
	if(argc<3){
		fprintf(stderr," Need more argument\n");
		return 1;
	}else if(argc==3){
		if(!isNum(argv[2])){
			fprintf(stderr," %s is not a number\n",argv[2]);
			return 2;
		}
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
	//create pipes
	int fd[numMapper][2];
	// int **fd = (int **)malloc(numMapper * sizeof(int *));
	//  for (int i=0; i<numMapper; i++)
	// 			fd[i] = (int *)malloc(2 * sizeof(int));
	pid_t pids[numMapper];
	//just make a function call to code in phase2.c
	//phase2 - Map Function
	//char * callMapper = "phase2";
	char * dicName =  malloc(SIZE_TXTPATH*sizeof(char));;
	int * buff = malloc(ALPHA_NUM_SIZE*sizeof(int)+1);
	int parentID = getpid();
	for (int i = 0; i < numMapper; i++) {
		if(pipe(fd[i])==-1){
			fprintf(stderr,"Fail to pipe\n");
			return 4;
		}
	}

	for (int i = 0; i < numMapper; i++) {

			pids[i]=fork();
			if (pids[i]<0) {
				printf("%s\n","here" );
				fprintf(stderr,"Fail to folk\n");
				return 4;
			}else if(pids[i]==0){
				sprintf(dicName,"MapperInput/Mapper_%d.txt",i);
				mapperPhase(dicName,fd[i]);
				// close(fd[i][0]);
				// write(fd[i][1],alphaCount, ALPHA_NUM_SIZE*sizeof(int)+1);
				// for (int i = 0; i < 26; i++) {
				// 	printf("%d:%d\n",i,alphaCount[i] );
				// }
				// close(fd[i][1]);
				free(dicName);
			}else{
				wait(NULL);
				reduceResult(fd, numMapper);
				break;
			}

	}
	//just make a function call to code in phase3.c
	//phase3 - Reduce Function
	//
	//phase4
	//wait for all processes to reach this point

	//just make a function call to code in phase4.c
	//master process reports the final output

	return 0;

}
