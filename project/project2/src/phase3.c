#include "phase3.h"


// You are free to use your own logic. The following points are just for getting started
/* Reduce Function
	1)	The reducer receives the list from 'm' mappers via pipes
	2)	Ensure proper closing of pipe ends
	3) 	Combine the lists to create a single list
	4) 	Write the list to "ReducerResult.txt" in the current folder
*/

void reduceResult(int (*fd)[2], int numOfMapper) {
  pid_t pids[numOfMapper];
  //int n = getpid();
  int * buff= malloc(ALPHA_NUM_SIZE*sizeof(int)+1);
  val * fia= malloc(ALPHA_NUM_SIZE*sizeof(val));
  int * store = malloc(ALPHA_NUM_SIZE*sizeof(int));
  for (int i = 0; i < numOfMapper; i++) {
    if ((pids[i]=fork())<0) {
    }else if(pids[i]==0){
      close(fd[i][1]);
      read(fd[i][0],buff,27*sizeof(int));
      // //read(fd[0+2*(i+1)],store,sizeof(store));
      // for (int i = 0; i < 26; i++) {
      //
      // }
      for(int j =0;j < 26; j++){
          printf("p3:%d:%d\n",j,buff[j]);
          store[j]=buff[j]+store[j];
      }
      close(fd[i][0]);
    }else{
      wait(NULL);
    }
  }
  // int status;
  // pid_t pid;
  // while(n>0){
  //   pid=wait(&status);
  //   --n;
  // }
  FILE * fp = fopen("ReducerResult.txt", "w");
  //filename = malloc(SIZE_TXTPATH*sizeof(char));

  if(fp == NULL){
    printf("Unable to create file.\n");
    exit(EXIT_FAILURE);
  }else{
      char * result = malloc(SIZE_TXTPATH*sizeof(char));
      int c = 0;
      for(int i = 0;i<26;i++){
        result[0] ='\0';
        c=i+65;
        char alphabet = c;
        printf("fput  %d:%d\n",i,store[i] );
        sprintf(result,"%c: %d\n",alphabet,store[i]);
        fputs(result,fp);
      }

  }
}
