 #include "phase4.h"
// You are free to use your own logic. The following points are just for getting started
/* Final Result
	1)	The master process reads the "ReducerResult.txt"
	2) 	Print the contents to standard output which is redirected to "FinalResult.txt"
			i.e., on using printf, the statement should be written to "FinalResult.txt"
*/
int finalResult(int *fd){
//FILE * fd;
    int * final;
    final = malloc(ALPHA_NUM_SIZE*sizeof(val));
    close(fd[1]);
    read(fd[1],final,sizeof(final));
    FILE *fptr;
    //int fd2 = open("FinalResult.txt", O_WRONLY | O_CREAT);
    fptr = freopen("FinalResult.txt", "a+", stdout);
    //dup2(1, fd2);
//newfd=dup2(fd, STDIN_FILENO);
    return 0;
}

//int finalResult(int * fd){
//    int * final;
//    final = malloc(ALPHA_NUM_SIZE*sizeof(val));
//    close(fd[1]);
//    read(fd[0],final,sizeof(final));
//    FILE * fptr;
//    fptr = freopen("FinalResult.txt", "a+",stdout);
//    return 0;
//}
