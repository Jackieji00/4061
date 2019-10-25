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
    c=fgetc(fpTxt);
    if(isalpha(c)!=0){
      if(c<97){
        alphaCount[c-65]++;
      }else{
        alphaCount[c-97]++;
      }
    }
    while(!feof(fpTxt)){
      c=fgetc(fpTxt);
      if(c == '\n'||c=='\t'||c==' '||c=='\v'||c=='\r'||c=='\f'){
        c=fgetc(fpTxt);
        if(isalpha(c)!=0){
          if(c<97){
            alphaCount[c-65]++;
          }else{
            alphaCount[c-97]++;
          }
        }
      }
    }
  }else{
    printf("fail to read file %s\n",txtName);
  }
}
void mapperPhase(char * folderName,int fd[2]){
  FILE * fp;
  char * txtName;
  char c;
  int co = 0;
  int* alphaCount;
  alphaCount = malloc(ALPHA_NUM_SIZE*sizeof(int));
  if((fp = fopen(folderName,"r"))!=NULL){
    txtName = malloc(SIZE_TXTPATH*sizeof(char));
    while(c != EOF){
      c=fgetc(fp);
      strncat(txtName,&c,1);
      while((c=fgetc(fp))!='\n'&&c!=EOF){
        strncat(txtName,&c,1);
      }
      if(c == EOF){
        break;
      }
      wordcount(txtName,alphaCount);
      txtName[0] = '\0';
   }
   free(txtName);
  }else{
    printf("fail to read file %s\n",folderName);
  }
  fclose(fp);
  close(fd[0]);
  write(fd[1],alphaCount, ALPHA_NUM_SIZE*sizeof(int)+1);

  for (int i = 0; i < 26; i++) {
    printf("%d:%d\n",i,alphaCount[i] );
  }
  close(fd[1]);
}
