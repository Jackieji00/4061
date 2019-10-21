//#include "phase2.h"
// You are free to use your own logic. The following points are just for getting started
/* 	Map Function
	1)	Each mapper selects a Mapper_i.txt to work with
	2)	Creates a list of letter, wordcount from the text files found in the Mapper_i.txt
	3)	Send the list to Reducer via pipes with proper closing of ends
*/
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "variables.h"


void wordcount(char * txtName,int * alphaCount){
  FILE * fpTxt;
  int c;
  if(fpTxt = fopen(txtName,"r")!=NULL){
    while(!feof(fp)){
      c=fgetc(fp);
      if(isalpha(c)!=0){
        if(c<97){
          alphaCount[c-65]++;
        }else{
          alphaCount[c-97]++;
        }
      }
    }
  }else{
    printf("fail to read file %s\n",txtName);
  }
}
int mapperPhase(char * folderName,int * fd,int numOfMapper){
  FILE * fp;
  char * txtName;
  char c;
  int * alphaCount;
  alphaCount = malloc(ALPHA_NUM_SIZE*sizeof(int));
  if(fp = fopen(folderName,"r")!=NULL){
    while(!feof(fp)){
      txtName = malloc(SIZE_TXTPATH*sizeof(char));
      c=fgetc(fp);
      strcpy(txtName,c);
      c=fgetc(fp);
      while(c!='\n'){
        strcat(txtName,c);
        c=fgetc(fp);
      }
      wordcount(txtName,&alphaCount);
      free(txtName);
    }
  }else{
    printf("fail to read file %s\n",folderName);
  }
  fclose(fp);
  close(fd[0+2*numOfMapper]);
  write(fd[1+2*numOfMapper],alphaCount, sizeof(alphaCount)+1);
  return 0;
}
