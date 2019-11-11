/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h


void *producer(void *arg){

  struct condBuffer* cq = (struct condBuffer*) arg;
  struct buffer* q = (struct buffer*) malloc(sizeof(struct buffer));;
  pthread_mutex_lock(cq->mutex);
  FILE *fp,logfile;
  char line[1024];
  int lineNum =0;
  if(strcmp(option,"-p")==0){
    logfile=fopen("log.txt","a");
    fputs("producer\n",logfile);
  }
  if((fp = fopen(filename,"r")) != NULL){
    q = cq->q;
    if(fgets(line,1024,fp)!=NULL){
      q->vals=line;
      q->check=1;
      q->lineNum=lineNum;
      q=q->next;
      if(strcmp(option,"-p")==0){
        fprintf(logfile, "producer: %d\n",lineNum);
      }
      lineNum++;
    }
    end=TRUE;
    fclose(fp);
  }else{
      printf("fail to read file %s\n",argv[1]);
  }

  pthread_cond_signal(cq->cond);

  pthread_mutex_unlock(cq->mutex);
}
