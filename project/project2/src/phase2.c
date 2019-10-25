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
      if(c == '\n'||c=='\t'||c=='\b'){
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
void mapperPhase(char * folderName,int (*fd)[2],int numOfMapper){
  FILE * fp;
  char * txtName;
  char c;
  int * alphaCount;
  int co = 0;
  alphaCount = malloc(ALPHA_NUM_SIZE*sizeof(int));
  if((fp = fopen(folderName,"r"))!=NULL){
    txtName = malloc(SIZE_TXTPATH*sizeof(char));
    while(c != EOF){
    //  printf("%d:%s\n",co,txtName);
      c=fgetc(fp);
      // printf("%c\n",c );
      strncat(txtName,&c,1);
      //printf("strncpy: %s\n",txtName);
      while((c=fgetc(fp))!='\n'&&c!=EOF){
        strncat(txtName,&c,1);
      }
      if(c == EOF){
        break;
      }
    //  printf("txtName:%s\n",txtName );
      wordcount(txtName,alphaCount);
      //memset(txtName,0,strlen(txtName));
      txtName[0] = '\0';
   }
   free(txtName);
  }else{
    printf("fail to read file %s\n",folderName);
  }
  fclose(fp);
  close(fd[numOfMapper][0]);
  write(fd[numOfMapper][1],alphaCount, ALPHA_NUM_SIZE*sizeof(int)+1);
  for (int i = 0; i < 26; i++) {
    printf("%d:%d\n",i,alphaCount[i] );
  }
  close(fd[numOfMapper][1]);
}
