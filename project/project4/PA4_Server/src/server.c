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

int * azList;
int ** updateStatus;

void *socketThread(void *arg) {
    int clientfd= *(int*)arg;
    struct condBuffer* buffer = (struct condBuffer*) malloc(sizeof(struct condBuffer));
    read(clientfd,buffer,1024);
    if(updateStatus[buffer->mapperID][2]==0&&updateStatus[buffer->mapperID][0]==0&&buffer->requestCode==CHECKIN){
        updateStatus[buffer->mapperID][0]=buffer->mapperID;
        updateStatus[buffer->mapperID][2]=buffer->requestCode;
    }else if(buffer->requestCode==GET_AZLIST){
        send(clientfd,azList,sizeof(azList),0);
    }else if(buffer->requestCode==GET_MAPPER_UPDATES){
        send(clientfd,updateStatus[buffer->mapperID][2],sizeof(int),0)
    }else if(buffer->requestCode==GET_ALL_UPDATES){
        int sum = 0;
        for(int i =0;i<50;i++){
            sum+=updateStatus[i][1];
        }
    }else if(buffer->requestCode==UPDATE_AZLIST){

    }
    close(clientfd);
    printf("close connection from %s:%d\n",clientid,clientPort);
}


int main(int argc, char *argv[]) {

    int server_port;
    FILE * log;

    if (argc == 2) { // 1 arguments
        server_port = atoi(argv[1]);
    } else {
        printf("Invalid or less number of arguments provided\n");
        printf("./server <server Port>\n");
        exit(0);
    }
    // Server (Reducer) code
    //initialize azList array;
    azList = new int[26];
    for (int i = 0; i < 26; i++) {
        azList[i]=0;
    }
    //initialize updateStatus
    updateStatus = new int[32][3];
    for (int i = 0; i < 32; i++) {
        azList[i][0]=0;//mapperID
        azList[i][1]=0;//update
        azList[i][2]=-1;//CHECKIN/CHECKOUT
    }
    // Create a TCP socket.
  	int sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
     }else
        printf("Socket successfully created..\n");
  	// Bind it to a local address.
  	struct sockaddr_in servAddress;
  	servAddress.sin_family = AF_INET;
  	servAddress.sin_port = htons(server_port);
  	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);//TODO:??
    //Set all bits of the padding field to 0
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
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
      printf("open connection from %s:%d\n",clientid,clientPort);
      if( pthread_create(&condPool[i], NULL, socketThread, &clientfd) != 0 ){
          printf("Failed to create thread\n");
      }
      if( poolIndex >= 50){
          poolIndex = 0;
          while(i < 50){
            pthread_join(tid[poolIndex++],NULL);
          }
          poolIndex = 0;
      }
      break; //break the loop
    }

    return 0;
}
