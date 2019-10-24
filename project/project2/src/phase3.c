#include "phase3.h"


// You are free to use your own logic. The following points are just for getting started
/* Reduce Function
	1)	The reducer receives the list from 'm' mappers via pipes
	2)	Ensure proper closing of pipe ends
	3) 	Combine the lists to create a single list
	4) 	Write the list to "ReducerResult.txt" in the current folder
*/

void reduceResult(int **fd, int numOfMapper) {
  int * buff;
  int * store;
  val * fia;
  buff = malloc(ALPHA_NUM_SIZE*sizeof(int));
  store = malloc(ALPHA_NUM_SIZE*sizeof(int));
  fia = malloc(ALPHA_NUM_SIZE*sizeof(val));

  for(int i=0; i< numOfMapper;i++){
    close(fd[i][1]);
    read(fd[i][0],buff,sizeof(buff));
    //read(fd[0+2*(i+1)],store,sizeof(store));
    for(int j =0;j < 26; j++){
        store[j]=buff[j]+store[j];
    }
    close(fd[i][0]);
  }


  for(int j =0;j < 26; j++){
    fia[j].c= 65 + j;
    fia[j].num = store[j];
  }

  FILE * fp = fopen("ReducerResult.txt", "w+");
  //filename = malloc(SIZE_TXTPATH*sizeof(char));

  if(fp == NULL){
    printf("Unable to create file.\n");
    exit(EXIT_FAILURE);
  }else{
  fwrite(&fia,sizeof(val*),sizeof(fia),fp);//write the results
  }
}
