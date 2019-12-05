// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	


	while(1){

	int n;
 	socklen_t addrlen;
   
    printf("Client: ");
    fgets(buffer, MAXLINE - 1, stdin);

    sendto(sockfd, (const char *) buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    n = recvfrom(sockfd, (char *) buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &addrlen);
    buffer[n] = '\0';
    printf("Server : %s", buffer);


	}

	close(sockfd);
	return 0;
}
