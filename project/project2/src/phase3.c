#include "phase3.h"


// You are free to use your own logic. The following points are just for getting started
/* Reduce Function
	1)	The reducer receives the list from 'm' mappers via pipes
	2)	Ensure proper closing of pipe ends
	3) 	Combine the lists to create a single list
	4) 	Write the list to "ReducerResult.txt" in the current folder
*/

void reduceResult(int (*fd)[2], int numOfMapper) {
  //initialize variables
  pid_t pids[numOfMapper];
  int * buff= malloc(ALPHA_NUM_SIZE*sizeof(int)+1);
  int * store = malloc(ALPHA_NUM_SIZE*sizeof(int));
  for (int i = 0; i < 26; i++) {
    store[i]=0;
    buff[i]=0;
  }
  //create processes to read the pipe and store the value into buff
  // then add values in buff to the array store.
  for (int i = 0; i < numOfMapper; i++) {
    if ((pids[i]=fork())<0) {
      fprintf(stderr,"Fail to folk\n");
      exit(EXIT_FAILURE);
    }else if(pids[i]==0){
      close(fd[i][1]);
      read(fd[i][0],buff,27*sizeof(int));
      for(int j =0;j < 26; j++){
          store[j]=buff[j]+store[j];
      }
      close(fd[i][0]);
    }else{
      wait(NULL);
      _exit(0);
      }
    }
  //write what in the array to the file "ReducerResult.txt"
  FILE * fp = fopen("ReducerResult.txt", "w");
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
        sprintf(result,"%c: %d\n",alphabet,store[i]);
        fputs(result,fp);
      }
    }
    fclose(fp);
    return;
}
