#include "phase1.h"

// You are free to use your own logic. The following points are just for getting started
/* 	Data Partitioning Phase - Only Master process involved
	1) 	Create 'MapperInput' folder
	2) 	Traverse the 'Sample' folder hierarchy and insert the text file paths
		to Mapper_i.txt in a load balanced manner
	3) 	Ensure to keep track of the number of text files for empty folder condition

*/

int readFiles(char * folderName){
  DIR *dr = opendir(folderName);
  struct dirent * de;
  char subdir[1000];
  if (dr == NULL){
		printf("Could not open directory %s\n",folderName);
		return -1;
	}
  for (int i = 0;(de = readdir(dr)) != NULL; i++){
    //printf("%dfolderNamed:%s\nd_name:%s\n",strstr(de->d_name,".")==NULL,folderName,de->d_name);
    if (de->d_type==DT_DIR&&strstr(de->d_name,".")==NULL) {
			subdir[0] ='\0';
      //sprintf(subdir,"%s/%s",folderName,de->d_name);
			strcpy(subdir,folderName);
			strcat(subdir,"/");
			strcat(subdir,de->d_name);
      //printf("subdir%s\n",subdir);
		  readFiles(subdir);
    }else if(de->d_type!=DT_DIR&&strstr(de->d_name,"txt")!=NULL){
      //printf("fileNamede:%s\n",folderName );
      strcpy(lines[count], folderName);
      strcat(lines[count],"/");
      strcat(lines[count],de->d_name);
      strcat(lines[count],"\n");
      fileCount++;
      count++;
    }
  }
  closedir(dr);
  return 0;
}

int partitionPharse(char * folderName,int numMapper){
  int fileFail = readFiles(folderName);
  //printf("%s\n","h" );
  if(fileCount == 0){
    return 0;
  }
  char * dicName =  malloc(SIZE_TXTPATH*sizeof(char));;
  //printf("%s\n",dicName );
  sprintf(dicName,"%s/MapperInput",folderName);
  //printf("%s\n",dicName );
  int check = mkdir(dicName,0777);
  char * txtName;
  FILE * fp;
  int numFileInM = count/numMapper;
  int remainderFile = count%numMapper;
  int countRemainder = remainderFile;
  //printf("remainderFile:%d\n",remainderFile );
  if (check){
    printf("fail to make MapperInput directory\n");
    return 1;
  }else{
    for(int i = 0;i<numMapper;i++){
      txtName = malloc(SIZE_TXTPATH*sizeof(char));
      sprintf(txtName,"%s/Mapper_%d.txt",dicName,i);
      if((fp = fopen(txtName,"a"))!= NULL){
      //  printf("i:%d\n", i);
        if(countRemainder!=0) {
          for(int j = (numFileInM+1)*i;j<(numFileInM+1)*(i+1);j++){
          //  printf("%s\n",lines[j]);
            fputs(lines[j],fp);
          }
          countRemainder--;
        }else{
          for(int j = remainderFile+numFileInM*i;j<(numFileInM*(i+1)+remainderFile);j++){
            //printf("%s\n",lines[j]);
            fputs(lines[j],fp);
          }
        }
        fclose(fp);
      }else{
        printf("fail to make file %s\n",txtName);
      }
      free(txtName);
    }
    free(dicName);
  }
  return 0;
}
// int main(void) {
//   /* code */
//
//   partitionPharse("../Testcases/TestCase2",3);
//
//   return 0;
// }
