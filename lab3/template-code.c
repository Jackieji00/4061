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

	pid_t child_pid1 = fork();
	if(child_pid1 <0){
		printf("can't fork, error occured\n");
		exit(1);
	}else if (child_pid1 ==0){
		printf("I am child_1!\n");
		execl("/bin/echo","-c", "hello world!",NULL);
	}else{
		wait(NULL);
		pid_t child_pid2 = fork();
		char *arr[] = {"abgxfghd",argv[1],NULL};
		if(child_pid2 <0){
			printf("can't fork, error occured\n");
			exit(1);
		}else if (child_pid2 ==0){
			printf("I am child_2!\n");
			execv("./rtime.o",arr);
		}else{
			wait(NULL);
		}
	}

}
