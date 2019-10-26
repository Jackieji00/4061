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
//read the text file manually add the 1st word, then add every alphabet after the whitespace,
//newline, tab, etc. store the result in the variable alphaCount
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
//initialize everything
  FILE * fp;
  char * txtName;
  char c;
  int co = 0;
  int* alphaCount;
  alphaCount = malloc(ALPHA_NUM_SIZE*sizeof(int));
  for (int i = 0; i < 26; i++) {
    alphaCount[i]=0;
  }
//read mapper text file and get the address call the function wordcount() to count
  if((fp = fopen(folderName,"r"))!=NULL){
    txtName = malloc(SIZE_TXTPATH*sizeof(char));
    while(c != EOF){
      txtName[0] = '\0';
      c=fgetc(fp);
      strncat(txtName,&c,1);
      while((c=fgetc(fp))!='\n'&&c!=EOF){
        strncat(txtName,&c,1);
      }
      if(c == EOF){
        break;
      }
      wordcount(txtName,alphaCount);

   }
   free(txtName);
  }else{
    printf("fail to read file %s\n",folderName);
  }
  fclose(fp);
  //write the int array to the pipe 
  close(fd[0]);
  write(fd[1],alphaCount, 27*sizeof(int));
  close(fd[1]);
  _exit(1);
}
