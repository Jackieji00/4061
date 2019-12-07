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
pthread_mutex_t currentConn_lock;

void *socketThread(void *arg) {
    struct threadArg * buf = (struct threadArg *) arg;
    int clientfd= buf->clientfd;
    char * clientip = buf->clientip;
    int clientPort = buf->clientport;
    struct requestBuffer* buffer = (struct requestBuffer*) malloc(sizeof(struct requestBuffer));
    int bufferResponse[28];
    while(1){
        if(pthread_mutex_trylock(&currentConn_lock)==0){
            read(clientfd,buffer,1024);
            if(updateStatus[buffer->mapperID][US_IS_CHECKEDIN]==0||updateStatus[buffer->mapperID][US_IS_CHECKEDIN]==-1){
                if(buffer->requestCode==CHECKIN){
                    //printf("1:%d\n",buffer->mapperID );
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_OK;
                    bufferResponse[RSP_DATA]=buffer->mapperID;
                    printf("data:%d\n",bufferResponse[RSP_DATA]);
                    updateStatus[buffer->mapperID][US_MAPPER_PID]=buffer->mapperID;
                    updateStatus[buffer->mapperID][US_IS_CHECKEDIN]=buffer->requestCode;
                    write(clientfd,bufferResponse,sizeof(bufferResponse));
                    printf("[%d] CHECKIN\n",buffer->mapperID);
                }else{
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_NOK;
                    bufferResponse[RSP_DATA]=buffer->mapperID;
                    printf("Cannot process request command %d due to not checkin yet.\n", buffer->requestCode);
                }
            }else{
                //printf("%d\n", buffer->requestCode);
                if(buffer->requestCode==GET_AZLIST){
                    //printf("2:%d\n",buffer->mapperID );
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_OK;
                    for (int i = RSP_DATA; i < 28; i++) {
                        bufferResponse[i]=azList[i-RSP_DATA];
                    }
                    printf("[%d] GET_AZLIST\n",buffer->mapperID);
                }else if(buffer->requestCode==GET_MAPPER_UPDATES){
                    //printf("3:%d\n",buffer->mapperID );
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_OK;
                    bufferResponse[RSP_DATA]=updateStatus[buffer->mapperID][US_NUM_UPDATES];
                    printf("[%d] GET_MAPPER_UPDATES\n",buffer->mapperID);
                }else if(buffer->requestCode==GET_ALL_UPDATES){
                    //printf("4:%d\n",buffer->mapperID );
                    int sum = 0;
                    for(int i =0;i<50;i++){
                        if(updateStatus[i][0]==0){break;}
                        sum+=updateStatus[i][1];
                    }
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_OK;
                    bufferResponse[RSP_DATA]=sum;
                    printf("[%d] GET_ALL_UPDATES\n",buffer->mapperID);
                }else if(buffer->requestCode==UPDATE_AZLIST){
                    //printf("5:%d\n",buffer->mapperID );
                    printf("%s\n","hh");
                    updateStatus[buffer->mapperID][US_NUM_UPDATES]++;
                    int * data = malloc(26*sizeof(int));
                    data=buffer->data;
                    for(int i =0;i<26;i++){
                        azList[i]+=data[i];
                    }
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_OK;
                    bufferResponse[RSP_DATA]=buffer->mapperID;
                }else if(buffer->requestCode==CHECKOUT){
                    //printf("6:%d\n",buffer->mapperID );
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_OK;
                    bufferResponse[RSP_DATA]=buffer->mapperID;
                    updateStatus[buffer->mapperID][US_IS_CHECKEDIN]=CHECKOUT;
                    printf("[%d] CHECKOUT\n",buffer->mapperID);
                    break;
                }else{
                    bufferResponse[RSP_COMMAND_ID]=buffer->requestCode;
                    bufferResponse[RSP_CODE]=RSP_NOK;
                    bufferResponse[RSP_DATA]=buffer->mapperID;
                }
            }
            write(clientfd,bufferResponse,sizeof(bufferResponse));
            pthread_mutex_unlock(&currentConn_lock);
        }
        sleep(1);
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

      printf("open connection from %s:%d\n",arg->clientip,arg->clientport);
      if( pthread_create(&condPool[poolIndex++], NULL, socketThread, arg) != 0 ){
          printf("Failed to create thread\n");
      }
      if( poolIndex >= 50){
          poolIndex = 0;
          while(poolIndex < 50){
              pthread_join(condPool[poolIndex++],NULL);
          }
          poolIndex = 0;
      }
    }
    close(sock);
    return 0;
}
