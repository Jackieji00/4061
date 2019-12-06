#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "../include/protocol.h"
#define SIZE_TXTPATH 100
#define ALPHA_NUM_SIZE 27


FILE *logfp;


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



void createLogFile(void) {
    pid_t p = fork();
    if (p==0)
        execl("/bin/rm", "rm", "-rf", "log", NULL);

    wait(NULL);
    mkdir("log", ACCESSPERMS);
    logfp = fopen("log/log_client.txt", "w");
}


int main(int argc, char *argv[]) {
    int mappers;
    char folderName[100] = {'\0'};
    char *server_ip;
    int server_port;
    int request[28];

    if (argc == 5) { // 4 arguments
        strcpy(folderName, argv[1]);
        mappers = atoi(argv[2]);
        server_ip = argv[3];
        server_port = atoi(argv[4]);
        if (mappers > MAX_MAPPER_PER_MASTER) {
            printf("Maximum number of mappers is %d.\n", MAX_MAPPER_PER_MASTER);
            printf("./client <Folder Name> <# of mappers> <server IP> <server Port>\n");
            exit(1);
        }

    } else {
        printf("Invalid or less number of arguments provided\n");
        printf("./client <Folder Name> <# of mappers> <server IP> <server Port>\n");
        exit(1);
    }

    // create log file
    createLogFile();

    // phase1 - File Path Partitioning
    traverseFS(mappers, folderName);

    // Phase2 - Mapper Clients's Deterministic Request Handling
    pid_t pids[mappers];


   char * dicName =  malloc(SIZE_TXTPATH*sizeof(char));
   struct condBuffer* buffer = (struct condBuffer*) malloc(sizeof(struct condBuffer));
   buffer->requestCode=0;
   buffer->mapperID = 0;

   for (int i = 0; i < mappers; i++) {
    pids[i]=fork();

    if (pids[i]<0) {
      fprintf(stderr,"Fail to folk\n");
      return 4;
    }else if(pids[i]==0){


      int sockfd = socket(AF_INET , SOCK_STREAM , 0);

      struct sockaddr_in address;
      address.sin_family = AF_INET;
      address.sin_port = htons(server_port);
      address.sin_addr.s_addr = inet_addr(server_ip);


      buffer->requestCode=CHECKIN;
      buffer->mapperID = i;

      write(sockfd, buffer, REQUEST_MSG_SIZE);


      sprintf(dicName,"MapperInput/Mapper_%d.txt",i);
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
      buffer->requestCode=UPDATE_AZLIST;
      for(int i =0 ; i <26;i++){
        buffer->data[i]=alphaCount[i];
      }
      write(sockfd, buffer, REQUEST_MSG_SIZE);
      fclose(fp);
      free(dicName);
      buffer->requestCode=GET_AZLIST;
      read(sockfd, buffer, REQUEST_MSG_SIZE);

      buffer->requestCode=GET_MAPPER_UPDATES;
      read(sockfd, buffer, REQUEST_MSG_SIZE);

      buffer->requestCode=GET_ALL_UPDATES ;
      write(sockfd, buffer, REQUEST_MSG_SIZE);
      read(sockfd, buffer, REQUEST_MSG_SIZE);

      close(sockfd);



    }




	}
    // Phase3 - Master Client's Dynamic Request Handling (Extra Credit)


    fclose(logfp);
    return 0;

}
