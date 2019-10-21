// #include "phase1.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"
// You are free to use your own logic. The following points are just for getting started
/* 	Data Partitioning Phase - Only Master process involved
	1) 	Create 'MapperInput' folder
	2) 	Traverse the 'Sample' folder hierarchy and insert the text file paths
		to Mapper_i.txt in a load balanced manner
	3) 	Ensure to keep track of the number of text files for empty folder condition

*/

int count = 0;
int readFiles(char * folderName){
  DIR *dr = opendir(folderName);
  struct dirent * de;

  if (dr == NULL){
		printf("Could not open directory %s\n",folderName);
		return -1;
	}
for (int i = 0;(de = readdir(dr)) != NULL; i++){

    if (de->d_type==DT_DIR&&strstr(de->d_name,".")==NULL) {
      //printf("folderNamed:%s\n",folderName );
      char subdir[1000];
			subdir[0] ='\0';
			strcpy(subdir,folderName);
			strcat(subdir,"/");
			strcat(subdir,de->d_name);
      //printf("%s\n",subdir);
		  readFiles(subdir);
    }else if(de->d_type==DT_REG){
      // printf("folderNamede:%s\n",folderName );
      strncpy(lines[count], folderName, strlen(folderName));
      strcat(lines[count],"/");
      strcat(lines[count],de->d_name);
      strcat(lines[count],"\n");
      count++;
    }
  }
  closedir(dr);
  return 0;
}

int partitionPharse(char * folderName,int numMapper){
  int fileFail = readFiles(folderName);
  int check = mkdir("MapperInput",0777);
  char * txtName;
  FILE * fp;
  int numFileInM = count/numMapper;
  int remainderFile = count%numMapper;
  int countRemainder = remainderFile;
  printf("remainderFile:%d\n",remainderFile );
  if (check){
    printf("fail to make MapperInput directory\n");
		return -1;
  }else{
    for(int i = 0;i<numMapper;i++){
      txtName = malloc(SIZE_TXTPATH*sizeof(char));
      sprintf(txtName,"MapperInput/Mapper_%d.txt",i);
      if((fp = fopen(txtName,"a"))!= NULL){
        printf("i:%d\n", i);
        if(countRemainder!=0) {
          for(int j = (numFileInM+1)*i;j<(numFileInM+1)*(i+1);j++){
            printf("%s\n",lines[j]);
            fputs(lines[j],fp);
          }
          countRemainder--;
        }else{
          for(int j = remainderFile+numFileInM*i;j<(numFileInM*(i+1)+remainderFile);j++){
            printf("%s\n",lines[j]);
            fputs(lines[j],fp);
          }
        }
        fclose(fp);
      }else{
        printf("fail to make file %s\n",txtName);
      }

    }
  }

  return 0;
}
int main(void) {
  /* code */
  partitionPharse("obj",3);

  return 0;
}
