#include "main.h"

int main(int argc, char *argv[]){

	// argument count check

	//just make a function call to code in phase1.c
	//phase1 - Data Partition Phase

	//create pipes

	//just make a function call to code in phase2.c
	//phase2 - Map Function

	//just make a function call to code in phase3.c
	//phase3 - Reduce Function

	//phase4
	//wait for all processes to reach this point

	//just make a function call to code in phase4.c
	//master process reports the final output
	if(argc<3){
		fprintf(stderr," Need more argument\n");
		return 1;
	}else if(argc==3){

	}else{
		fprintf(stderr,"Too much arguements\n");
		return 2;
	}
	return 0;

}
