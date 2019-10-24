#include "phase2.h"
// You are free to use your own logic. The following points are just for getting started
/* 	Map Function
	1)	Each mapper selects a Mapper_i.txt to work with
	2)	Creates a list of letter, wordcount from the text files found in the Mapper_i.txt
	3)	Send the list to Reducer via pipes with proper closing of ends
*/


void wordcount(char * txtName,int * alphaCount){
  FILE * fpTxt;
  int c;
  if((fpTxt = fopen(txtName,"r"))!=NULL){
    while(!feof(fpTxt)){
      c=fgetc(fpTxt);
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
void mapperPhase(char * folderName,int ** fd,int numOfMapper){
  FILE * fp;
  char * txtName;
  char c;
  int * alphaCount;
  alphaCount = malloc(ALPHA_NUM_SIZE*sizeof(int));
  if((fp = fopen(folderName,"r"))!=NULL){
    while(!feof(fp)){
      txtName = malloc(SIZE_TXTPATH*sizeof(char));
      c=fgetc(fp);
      strncpy(txtName,&c,1);
      c=fgetc(fp);
      while(c!='\n'){
        strncat(txtName,&c,1);
        c=fgetc(fp);
      }
      wordcount(txtName,alphaCount);
      free(txtName);
   }
  }else{
    printf("fail to read file %s\n",folderName);
  }
  fclose(fp);
  close(fd[numOfMapper][0]);
  write(fd[numOfMapper][1],alphaCount, sizeof(alphaCount)+1);
  close(fd[numOfMapper][1]);
}
