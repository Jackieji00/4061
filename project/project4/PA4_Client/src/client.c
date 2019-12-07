#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "../include/protocol.h"
#define SIZE_TXTPATH 100

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
        return;
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

    if (argc == 5) { // 4 arguments
        strcpy(folderName, argv[1]);
        printf("hh%s\n",folderName );
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
    for (int i = 0; i < mappers; i++) {
        pids[i]=fork();
        //fail
        if (pids[i]<0) {
            fprintf(stderr,"Fail to folk\n");
            return 4;
        }else if(pids[i]==0){
            //buffer initialize
            struct requestBuffer* buffer = (struct requestBuffer*) malloc(sizeof(struct requestBuffer));
            int * buffer_response = malloc(28*sizeof(int));
            buffer->requestCode=0;
            buffer->mapperID = i+1;

            int sockfd = socket(AF_INET , SOCK_STREAM , 0);

            if (sockfd == -1) {
                printf("socket creation failed...\n");
                exit(0);
            }else{
                printf("[%d] Socket successfully created..%d\n",i,getpid());
            }

            struct sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_port = htons(server_port);
            address.sin_addr.s_addr = inet_addr(server_ip);
            //Set all bits of the padding field to 0
            memset(address.sin_zero, '\0', sizeof(address.sin_zero));

            if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0) {
                //connecting

                char logConnect[255];
                sprintf(logConnect, "[%d] open connection\n",buffer->mapperID );
                fputs(logConnect, logfp);
                //debug
                printf("%s",logConnect);

                //checkin
                for(int j =0;j<26;j++){
                    buffer->data[j]=0;
                }
                buffer->requestCode=CHECKIN;
                write(sockfd, buffer, sizeof(buffer));
                read(sockfd, buffer_response, 28*sizeof(int));
                printf( "[%d] CHECKIN: %d %d\n",buffer->mapperID,buffer_response[RSP_CODE],buffer_response[RSP_DATA]);
                char logCheckin[255];
                sprintf(logCheckin, "[%d] CHECKIN: %d %d\n",buffer->mapperID,buffer_response[RSP_CODE],buffer_response[RSP_DATA]);
                fputs(logCheckin, logfp);
                //reset struct
                //memset(&buffer_response, 0, sizeof(buffer_response));

                //update
                buffer->requestCode=UPDATE_AZLIST;
                char * dicName =  malloc(SIZE_TXTPATH*sizeof(char));
                sprintf(dicName,"MapperInput/Mapper_%d.txt",i+1);
                FILE *fp;
                char * txtName;
                char c='\0';
                int* alphaCount;
                int messageCount=0;
                alphaCount = malloc(ALPHABETSIZE*sizeof(int));
                for (int j = 0; j < 26; j++) {
                    alphaCount[j]=0;
                }
                printf("%s\n",dicName );
                if((fp = fopen(dicName,"r"))!=NULL){
                    txtName = malloc(SIZE_TXTPATH*sizeof(char));
                    while(1){
                        c=fgetc(fp);
                        while(c!='\n'&&c!=EOF){
                            strncat(txtName,&c,1);
                            c=fgetc(fp);
                        }
                        //printf("textname: %s\n", txtName);
                        wordcount(txtName,alphaCount);
                        if(feof(fp)||strstr(txtName,"/")==NULL){break;}
                        messageCount++;
                        for(int j=0;j<26;j++){
                            buffer->data[j] = alphaCount[j];
                        }
                        write(sockfd, buffer, REQUEST_MSG_SIZE);
                        read(sockfd,buffer_response,sizeof(buffer_response));
                        char logUpdate[255];
                        sprintf(logUpdate, "[%d] UPDATE_AZLIST: %d\n",buffer->mapperID,messageCount);
                        fputs(logUpdate,logfp);
                        printf("[%d] UPDATE_AZLIST: %d\n",buffer->mapperID,messageCount);
                        //printf("[%d] textname: %s\n",i+1,txtName);
                        //printf("[%d] dicName: %s\n",i+1,dicName);
                        //reset
                        logUpdate[0]='\0';
                        txtName[0] = '\0';
                        //memset(&buffer_response, 0, sizeof(buffer_response));

                    }
                    free(txtName);
                    fclose(fp);
                }else{
                    printf("fail to read file %s\n",dicName);
                    fclose(fp);

                }
                free(dicName);

                //get azList
                buffer->requestCode=GET_AZLIST;
                
                for(int i =0 ; i <26;i++){
                    buffer->data[i]=0;
                }
                write(sockfd, buffer, sizeof(buffer));
                read(sockfd, buffer_response, 28*sizeof(int));
                char numbers[100];
                int index =0;
                for(int i =0;i<26;i++){
                    printf("%d: %d\n",i,buffer_response[RSP_DATA+i]);
                    index += sprintf(&numbers[index], "%d ",buffer_response[RSP_DATA+i]);
                }
                printf( "[%d] GET_AZLIST: %d <%s>\n",buffer->mapperID,buffer_response[RSP_CODE], numbers);
                char logGetList[255];
                sprintf(logGetList, "[%d] GET_AZLIST: %d <%s>\n",buffer->mapperID,buffer_response[RSP_CODE], numbers);
                fputs(logGetList, logfp);
    //       //
    //       // buffer->requestCode=GET_MAPPER_UPDATES;
    //       // for(int i =0 ; i <26;i++){
    //       //   buffer->data[i]=0;
    //       // }
    //       // write(sockfd, buffer, REQUEST_MSG_SIZE);
    //       // read(sockfd, buffer_response, RESPONSE_MSG_SIZE);
    //       // printf("[%d] GET_MAPPER_UPDATES: %d %d\n",buffer->mapperID,buffer_response->responseCode,*buffer_response->data );
    //       // char dd[255];
    //       // sprintf(dd, "[%d] GET_MAPPER_UPDATES: %d %d\n",buffer->mapperID,buffer_response->responseCode,*buffer_response->data );
    //       // fputs(dd, logfp);
    //       //
    //       // buffer->requestCode=GET_ALL_UPDATES ;
    //       // write(sockfd, buffer, REQUEST_MSG_SIZE);
    //       // read(sockfd, buffer_response, RESPONSE_MSG_SIZE );
    //       // printf("[%d] GET_ALL_UPDATES: %d %d\n",buffer->mapperID,buffer_response->responseCode,*buffer_response->data );
    //       // char ee[255];
    //       // sprintf(ee, "[%d] GET_ALL_UPDATES: %d %d\n",buffer->mapperID,buffer_response->responseCode,*buffer_response->data );
    //       // fputs(ee, logfp);
    //       //
                buffer->requestCode=CHECKOUT ;
                write(sockfd, buffer, REQUEST_MSG_SIZE);
                read(sockfd, buffer_response, RESPONSE_MSG_SIZE);
                printf("[%d] CHECKOUT: %d %d\n",buffer->mapperID,buffer_response[RSP_CODE],buffer_response[RSP_DATA] );
                char logCheckout[255];
                sprintf(logCheckout, "[%d] CHECKOUT: %d %d\n",buffer->mapperID,buffer_response[RSP_CODE],buffer_response[RSP_DATA] );
                fputs(logCheckout, logfp);
                //close connection
                // wait(NULL);
                printf("[%d] close connection\n", buffer->mapperID);
                close(sockfd);
                _exit(1);
            }else {
                perror("Connection failed!");
      	    }

        }
    }
    wait(NULL);
    // Phase3 - Master Client's Dynamic Request Handling (Extra Credit)


    fclose(logfp);
    _exit(0);
    return 0;

}
