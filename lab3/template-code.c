#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}
	char n[] = argv[1];

	pid_t child_pid1 = fork();
	if(child_pid1 <0){
		printf("can't fork, error occured\n");
		exit(1);
	}else if (child_pid1 ==0){
		execl("/bin/bash","-c","echo \"hello world!\"",NULL);
		exit(1);
	}else{

	}
	pid_t child_pid2 = fork();
	const char *arr[] = {"-o",n,NULL}
	if(child_pid2 <0){
		printf("can't fork, error occured\n");
		exit(1);
	}else if (child_pid2 ==0){
		execv("rtime.o",n);
		exit(1);
	}
}
