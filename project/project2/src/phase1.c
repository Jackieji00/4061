#include "phase1.h"

// You are free to use your own logic. The following points are just for getting started
/* 	Data Partitioning Phase - Only Master process involved
	1) 	Create 'MapperInput' folder
	2) 	Traverse the 'Sample' folder hierarchy and insert the text file paths
		to Mapper_i.txt in a load balanced manner
	3) 	Ensure to keep track of the number of text files for empty folder condition

*/

int readFiles(char * folderName){
//open the big directory and check if the next file is a folder or not;
//if it is, continue open the folder until meet the  1st text file
//if it is not , then copy the file link to the global variable lines
  DIR *dr = opendir(folderName);
  struct dirent * de;
  char subdir[1000];
  if (dr == NULL){
		printf("Could not open directory %s\n",folderName);
		return -1;
	}
  for (int i = 0;(de = readdir(dr)) != NULL; i++){
    if (de->d_type==DT_DIR&&strstr(de->d_name,".")==NULL) {
			subdir[0] ='\0';
			strcpy(subdir,folderName);
			strcat(subdir,"/");
			strcat(subdir,de->d_name);
		  readFiles(subdir);
    }else if(de->d_type!=DT_DIR&&strstr(de->d_name,"txt")!=NULL){
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
  //stop if it is empty folder
  if(fileCount == 0){
    return 0;
  }
  char * dicName =  "MapperInput";
  int check = mkdir(dicName,0777);
  char * txtName;
  FILE * fp;
  int numFileInM = count/numMapper;
  int remainderFile = count%numMapper;
  int countRemainder = remainderFile;
  if (check){
    printf("fail to make MapperInput directory\n");
    return 1;
  }else{
//make the number of text for mapper ,and even divide the addresses to
//the text file, if there is a remainder, then evenly divide add 1 more address to
//each text file until no remainder file left
    for(int i = 0;i<numMapper;i++){
      txtName = malloc(SIZE_TXTPATH*sizeof(char));
      sprintf(txtName,"%s/Mapper_%d.txt",dicName,i);
      if((fp = fopen(txtName,"a"))!= NULL){
        if(countRemainder!=0) {
          for(int j = (numFileInM+1)*i;j<(numFileInM+1)*(i+1);j++){
            fputs(lines[j],fp);
          }
          countRemainder--;
        }else{
          for(int j = remainderFile+numFileInM*i;j<(numFileInM*(i+1)+remainderFile);j++){
            fputs(lines[j],fp);
          }
        }
        fclose(fp);
      }else{
        printf("fail to make file %s\n",txtName);
      }
      free(txtName);
    }
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
