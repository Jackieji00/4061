//#include "phase3.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"
// You are free to use your own logic. The following points are just for getting started
/* Reduce Function
	1)	The reducer receives the list from 'm' mappers via pipes
	2)	Ensure proper closing of pipe ends
	3) 	Combine the lists to create a single list
	4) 	Write the list to "ReducerResult.txt" in the current folder
*/
typedef struct value{
    char c;
    int num;
}val;


int * store;
val * fia;
int * buff = malloc(ALPHA_NUM_SIZE*sizeof(int));
store = malloc(ALPHA_NUM_SIZE*sizeof(int));
fia = malloc(ALPHA_NUM_SIZE*sizeof(val));
int *reduceResult(int *fd, int numOfMapper) {
    for(int i=0; i< numOfMapper;i++){
        read(fd[0+2*i],buff,sizeof(buff));
        //read(fd[0+2*(i+1)],store,sizeof(store));
    for(int j =0;j < 26; j++){
        store[j]=buff[j]+store[j];
    }
        close(fd[0+2*i]);}
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
    }
else{
    fwrite(&fia,sizeof(val*),sizeof(fia),fp)//write the results
}

