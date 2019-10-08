#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void printStats(char* path, int max) {

	// TODO: Put code here.
	DIR *dr = opendir(path);
	struct dirent * de;
	if (dr == NULL){
		printf("Could not open current directory" );
		return;
	}
	for (int i = 0; i < max&&(de = readdir(dr)) != NULL; i++) {
		if(de->d_type== DT_DIR){
			char subdir[1000];
			subdir[0] ='\0';
			strcpy(subdir,path);
			strcat(subdir,"/");
			strcat(subdir,de->d_name);
			printStats(subdir,max);
		}else{
			struct stat *buf = malloc(sizeof(struct stat *));
			printf("%s/%s\n",path,de->d_name);
			stat(de->d_name,buf);
			printf("User id of owner: %u\nTotal size in bytes: %lld\n",buf->st_uid,buf->st_size);
			free(buf);
		}
	}


}

int main(int argc, char** argv) {

	if (argc < 3) {

		printf("Wrong number of args, expected 2, given %d", argc - 1);
		exit(1);
	}

	printStats(argv[1], atoi(argv[2]));
}
