/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h
int end = FALSE;
int alphaCount[25];
char * option ="--";
int main(int argc, char *argv[]){

  if(argc<3){// 4 for no extra credits
    fprintf(stderr," Need more argument\n");
    return 1;
  }else if(argc==3||argc == 4){
    FILE *fp;
    if((fp = fopen(argv[2],"r"))== NULL){
      fprintf(stderr, "%s is invalid\n",argv[2] );
      return 2;
    }else{
      if(argc==4&&strcmp(argv[3],"-p")==0){
         strcpy(option,"-p");
      }else if(argc==4&&(strcmp(argv[3],"-b")==0||strcmp(argv[3],"-bp")==0)){
        fprintf(stderr, "extra credit is not supported\n");
        return 3;
      }else if(argc==4){
        fprintf(stderr, "%s is invalid\n",argv[3]);
        return 3;
      }

      int numCosumer = atoi(argv[1]);
      FILE * logfile;
      // Create threads.

      pthread_t condPool[numCosumer+1];
      struct condBuffer* cq = (struct condBuffer*) malloc(sizeof(struct condBuffer));
    	cq->q = (struct buffer*) malloc(sizeof(struct buffer));
    	cq->q->next=(struct buffer*) malloc(sizeof(struct buffer));
      cq->q->check=0;//0 for initialize
      cq->filename = malloc(100*sizeof(char));//assume the max filename is 100
      strcpy(cq->filename,argv[2]);
      cq->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    	cq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    	pthread_cond_init(cq->cond, NULL);
    	pthread_mutex_init(cq->mutex, NULL);
    	// Launch them.

      pthread_create(&condPool[0], NULL, producer, (void*) cq);

    	for (int i=0; i < numCosumer; ++i) {
        cq->consumerId=i;
    		pthread_create(&condPool[1 + i], NULL, consumer, (void*) cq); //start consumer threads
    	}

    	for (int i=0; i < numCosumer; ++i){
        pthread_join(condPool[i], NULL); //wait for all the threads to be finished
      }
      printf("%s\n","here");
      finilize();
    }
  }else{
    fprintf(stderr,"Too much arguements\n");
    return 3;
  }
  	return 0;
}
