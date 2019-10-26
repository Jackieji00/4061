 #include "phase4.h"
// You are free to use your own logic. The following points are just for getting started
/* Final Result
	1)	The master process reads the "ReducerResult.txt"
	2) 	Print the contents to standard output which is redirected to "FinalResult.txt"
			i.e., on using printf, the statement should be written to "FinalResult.txt"
*/
void finalResult(void){
  int fin,fout;
  if(fin = open("FinalResult.txt",O_WRONLY|O_CREAT|O_TRUNC,0600)!=-1){
    perror("fail to open FinalResult.txt");
    exit(2);
  }
  dup2(fin,1);
  close(fin);
  execlp("cat","cat","ReducerResult.txt",NULL);
  return;
}
