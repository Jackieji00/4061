 #include "phase4.h"
// You are free to use your own logic. The following points are just for getting started
/* Final Result
	1)	The master process reads the "ReducerResult.txt"
	2) 	Print the contents to standard output which is redirected to "FinalResult.txt"
			i.e., on using printf, the statement should be written to "FinalResult.txt"
*/
void finalResult(void){
   // int fout,fin;
   // if ((fout = open("FinalResult.txt",O_WRONLY | O_CREAT | O_TRUNC, 0600))== -1){
   //    perror("open outfile");
   //    exit(2);
   // }
   // if ((fin = open("FinalResult.txt",O_RDONLY)== -1){
   //    perror("open infile");
   //    exit(2);
   // }
   // close(fileno(stdout));
  // pid_t pid;
  // int fptr,fphase3;
  // pid = fork();
  // if(pid==0){
  //   if((fptr = open("FinalResult.txt", O_WRONLY|O_CREAT|O_TRUNC,0600))==-1){
  //     perror("open outfile");
  //     exit(3);
  //   }
  //   dup2(fptr, 1);
  //
  //   execlp("cat","cat","ReducerResult.txt",NULL);
  //   close(fptr);
  // }else if (pid>0){
  //   wait(NULL);
  //   _exit(1);
  // }
  FILE* fileIn = fopen("ReducerResult.txt", "r");
	if (!fileIn) {
		printf("Failed to open the reducer file\n");
		return;
	}

	//FILE* fileOut = fopen("FinalResult.txt", "w");
	int fileOut = open("FinalResult.txt", O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if (!fileOut) {
		printf("Failed to create the final file\n");
		fclose(fileIn);
		return;
	}

	close(STDOUT_FILENO);
	//fprintf(stderr, "fileout: %d\n", fileOut);
	//perror("fopen");
	//if(dup2(fileOut, STDOUT_FILENO) == -1) {
	if(dup(fileOut) == -1) {
		//fprintf(stderr, "Failed to redirect stdout\n");
		//perror("Dup");
		fclose(fileIn);
		close(fileOut);
		return;
	}

	char buf[1024] = "";
	while (fgets(buf, 1024, fileIn)){
		printf("%s", buf);
	}

	fclose(fileIn);
	close(fileOut);
 //  FILE *fp = fopen("FinalResult.txt", "w");
 // if (fp == NULL) {
 //   perror("Failure: ");
 //   return;
 // }
 // fclose(fp);
 //
 // int fd = open("FinalResult.txt", O_CREAT|O_RDWR);
 //
 // int fd2 = open("ReducerResult.txt", O_RDONLY);
 //
 // dup2(fd, STDOUT_FILENO);
 // char buf[1000];
 // int len;
 // while((len = read(fd2, buf, 1000)) > 0)
 // {
 //   printf("%s\n", buf);
 // }
 // close(fd);
 // close(fd2);

    //close(fptr);


    // FILE * fpTxt;
    // char c;
    // if((fpTxt = fopen("ReducerResult.txt","r"))!=NULL){
    //   while(c != EOF){
    //     c=fgetc(fpTxt);
    //     printf("%c",c);
    //   }
    // }else{
    //   printf("fail to read file \n");
    // }
    return;
}
