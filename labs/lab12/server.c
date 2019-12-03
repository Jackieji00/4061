#define _DEFAULT_SOURCE
#define NUM_ARGS 0

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 4061
#define MAX_CONCURRENT 5
#define MAX_MSG_SIZE 1000

int currentConn = 0;
pthread_mutex_t currentConn_lock;

struct threadArg {
	int clientfd;
	char * clientip;
	int clientport;
};

void * threadFunction(void * arg) {
	struct threadArg * tArg = (struct threadArg *) arg;
	char readbuf[MAX_MSG_SIZE];
	
	read(tArg->clientfd, readbuf, MAX_MSG_SIZE);
	printf("%s\n", readbuf);
	write(tArg->clientfd, (void *) "Acknowledge", 12);
	close(tArg->clientfd);
	free(tArg);
	pthread_mutex_lock(&currentConn_lock);
	currentConn--;
	pthread_mutex_unlock(&currentConn_lock);
	return NULL;
}

int main(int argc, char** argv) {

	if (argc > NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}
	
	pthread_t threads[MAX_CONCURRENT];
	int count = 0;
	pthread_mutex_init(&currentConn_lock, NULL);
	
	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);

	// Bind it to a local address.
	struct sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(SERVER_PORT);
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (struct sockaddr *) &servAddress, sizeof(servAddress));

	// We must now listen on this port.
	listen(sock, MAX_CONCURRENT);

	// A server typically runs infinitely, with some boolean flag to terminate.
	while (1) {

		// Now accept the incoming connections.
		struct sockaddr_in clientAddress;

		socklen_t size = sizeof(struct sockaddr_in);
		int clientfd = accept(sock, (struct sockaddr*) &clientAddress, &size);

        struct threadArg *arg = (struct threadArg *) malloc(sizeof(struct threadArg));
		
		arg->clientfd = clientfd;
		arg->clientip = inet_ntoa(clientAddress.sin_addr);
		arg->clientport = clientAddress.sin_port;
		
		//TODO: Handle the accepted connection by passing off functionality to a thread
		//      Up to MAX_CONCURRENT threads can be running simultaneously, so you will
		//      have to decide how to ensure that this condition holds.


        pthread_mutex_lock(&currentConn_lock);
		if(currentConn == MAX_CONCURRENT) {
			printf("Server is too busy\n");
			close(clientfd);
			free(arg);
			continue;
        }else{
            
            if( pthread_create(&threads[count++], NULL, threadFunction, arg) != 0 )
                printf("Failed to create thread\n");
            
            
            currentConn++;
            
            if(count == MAX_CONCURRENT)
                count = 0;
        }
        pthread_mutex_unlock(&currentConn_lock);
	}

	// Close the socket.
	close(sock);
}
