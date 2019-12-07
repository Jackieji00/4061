#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <zconf.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include "../include/protocol.h"

int azList[26];
int updateStatus[50][3];
int currentConn = 0;
pthread_mutex_t currentConn_lock;

void *socketThread(void *arg) {
    struct threadArg * buf = (struct threadArg *) arg;
    int clientfd= buf->clientfd;
    char * clientip = buf->clientip;
    int clientPort = buf->clientport;
    int buffer[28];
    int bufferResponse[28];
    while(1){
        read(clientfd,buffer,28*sizeof(int));
        printf("[%d]:%d\n",buffer[RQS_MAPPER_PID],buffer[RQS_COMMAND_ID]);
        if(updateStatus[buffer[RQS_MAPPER_PID]][US_IS_CHECKEDIN]==0||updateStatus[buffer[RQS_MAPPER_PID]][US_IS_CHECKEDIN]==-1){
            if(buffer[RQS_COMMAND_ID]==CHECKIN){
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_OK;
                bufferResponse[RSP_DATA]=buffer[RQS_MAPPER_PID];
                updateStatus[buffer[RQS_MAPPER_PID]][US_MAPPER_PID]=buffer[RQS_MAPPER_PID];
                updateStatus[buffer[RQS_MAPPER_PID]][US_IS_CHECKEDIN]=buffer[RQS_COMMAND_ID];
                //write(clientfd,bufferResponse,28*sizeof(int));
                printf("[%d] CHECKIN\n",buffer[RQS_MAPPER_PID]);
            }else{
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_NOK;
                bufferResponse[RSP_DATA]=buffer[RQS_MAPPER_PID];
                printf("Cannot process request command %d due to not checkin yet.\n", buffer[RQS_COMMAND_ID]);
            }
        }else{
            //printf("%d\n", buffer[RQS_COMMAND_ID]);
            if(buffer[RQS_COMMAND_ID]==GET_AZLIST){
                printf("2:%d\n",buffer[RQS_MAPPER_PID] );
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_OK;
                for (int i = 0; i < 26; i++) {
                    //pthread_mutex_lock(&currentConn_lock);
                    bufferResponse[i+RSP_DATA]=azList[i];
                    printf("%d\n", azList[i]);
                    //pthread_mutex_unlock(&currentConn_lock);
                }
                printf("[%d] GET_AZLIST\n",buffer[RQS_MAPPER_PID]);
                //write(clientfd,bufferResponse,28*sizeof(int));
                printf("%s\n","hh" );
            }else if(buffer[RQS_COMMAND_ID]==GET_MAPPER_UPDATES){
                //printf("3:%d\n",buffer[RQS_MAPPER_PID] );
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_OK;
                bufferResponse[RSP_DATA]=updateStatus[buffer[RQS_MAPPER_PID]][US_NUM_UPDATES];
                printf("[%d] GET_MAPPER_UPDATES\n",buffer[RQS_MAPPER_PID]);
            }else if(buffer[RQS_COMMAND_ID]==GET_ALL_UPDATES){
                //printf("4:%d\n",buffer[RQS_MAPPER_PID] );
                int sum = 0;
                for(int i =0;i<50;i++){
                    if(updateStatus[i][0]==0){break;}
                    sum+=updateStatus[i][1];
                }
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_OK;
                bufferResponse[RSP_DATA]=sum;
                printf("[%d] GET_ALL_UPDATES\n",buffer[RQS_MAPPER_PID]);
            }else if(buffer[RQS_COMMAND_ID]==UPDATE_AZLIST){
                //printf("5:%d\n",buffer[RQS_MAPPER_PID] );
                updateStatus[buffer[RQS_MAPPER_PID]][US_NUM_UPDATES]++;
                for(int i =0;i<26;i++){
                    //pthread_mutex_lock(&currentConn_lock);
                    azList[i]+=buffer[RQS_DATA+i];
                    //printf("%d\n",azList[i]);
                    //pthread_mutex_unlock(&currentConn_lock);
                }
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_OK;
                bufferResponse[RSP_DATA]=buffer[RQS_MAPPER_PID];
            }else if(buffer[RQS_COMMAND_ID]==CHECKOUT){
                //printf("6:%d\n",buffer[RQS_MAPPER_PID] );
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_OK;
                bufferResponse[RSP_DATA]=buffer[RQS_MAPPER_PID];
                updateStatus[buffer[RQS_MAPPER_PID]][US_IS_CHECKEDIN]=CHECKOUT;
                printf("[%d] CHECKOUT\n",buffer[RQS_MAPPER_PID]);
                break;
            }else{
                bufferResponse[RSP_COMMAND_ID]=buffer[RQS_COMMAND_ID];
                bufferResponse[RSP_CODE]=RSP_NOK;
                bufferResponse[RSP_DATA]=buffer[RQS_MAPPER_PID];
            }
        }
        write(clientfd,bufferResponse,28*sizeof(int));

    }
    printf("close connection from %s:%d\n",clientip,clientPort);
    close(clientfd);
    pthread_exit(NULL);
    printf("%s\n", "i don't know");
}

int main(int argc, char *argv[]) {

    int server_port;
    pthread_mutex_init(&currentConn_lock, NULL);

    if (argc == 2) { // 1 arguments
        server_port = atoi(argv[1]);
    } else {
        printf("Invalid or less number of arguments provided\n");
        printf("./server <server Port>\n");
        exit(0);
    }
    // Server (Reducer) code
    //initialize azList array;
    for (int i = 0; i < 26; i++) {
        azList[i]=0;
    }
    //initialize updateStatus
    for (int i = 0; i < 32; i++) {
        updateStatus[i][US_MAPPER_PID]=0;//mapperID
        updateStatus[i][US_NUM_UPDATES]=0;//update
        updateStatus[i][US_IS_CHECKEDIN]=-1;//CHECKIN/CHECKOUT
    }
    // Create a TCP socket.
  	int sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created..\n");
    }
  	// Bind it to a local address.
  	struct sockaddr_in servAddress;
  	servAddress.sin_family = AF_INET;
  	servAddress.sin_port = htons(server_port);
  	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    //Set all bits of the padding field to 0
    memset(servAddress.sin_zero, '\0', sizeof(servAddress.sin_zero));
  	bind(sock, (struct sockaddr *) &servAddress, sizeof(servAddress));

  	// We must now listen on this port.
  	if(listen(sock, MAX_CONCURRENT_CLIENTS)==0){
        printf("Server is now listening\n");
    }else{
        printf("listening is failed\n");
    }

    pthread_t condPool[MAX_CONCURRENT_CLIENTS];
    int poolIndex = 0;

    while (1) {
      // Now accept the incoming connections.
      struct sockaddr_in clientAddress;

      socklen_t size = sizeof(struct sockaddr_in);
      int clientfd = accept(sock, (struct sockaddr*) &clientAddress, &size);
      struct threadArg *arg = (struct threadArg *) malloc(sizeof(struct threadArg));

      arg->clientfd = clientfd;
      arg->clientip = inet_ntoa(clientAddress.sin_addr);
      arg->clientport = clientAddress.sin_port;

      pthread_mutex_lock(&currentConn_lock);
      if(currentConn == MAX_CONCURRENT_CLIENTS) {
          printf("Server is too busy\n");
          close(clientfd);
          free(arg);
          continue;
      }else{
          if( pthread_create(&condPool[poolIndex++], NULL, socketThread, arg) != 0 ){
              printf("Failed to create thread\n");
          }
          currentConn++;
          if(poolIndex == MAX_CONCURRENT_CLIENTS){
              poolIndex = 0;
          }
      }
      pthread_mutex_unlock(&currentConn_lock);
    }
    close(sock);
    return 0;
}
