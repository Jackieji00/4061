/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h


void * producer(void *arg){

  struct condBuffer* cq = (struct condBuffer*) arg;
  struct buffer* q = (struct buffer*) malloc(sizeof(struct buffer));;
  pthread_mutex_lock(cq->mutex);
  FILE *fp;
  FILE *logfile=fopen("log.txt","a");
  char line[1024];
  int lineNum =0;
  if(strcmp(option,"-p")==0){
    fputs("producer\n",logfile);
  }
  if((fp = fopen(cq->filename,"r")) != NULL){
    q = cq->q;
    while(fgets(line,1024,fp)!=NULL){
      strcpy(q->vals,line);
      q->check=1;
      q->lineNum=lineNum;
      q=q->next;
      if(strcmp(option,"-p")==0){
        fprintf(logfile, "producer: %d\n",lineNum);
      }
      lineNum++;
      line[0]='\0';
    }
    end=TRUE;
    fclose(fp);
  }else{
      printf("fail to read file %s\n",cq->filename);
  }

  pthread_cond_signal(cq->cond);

  pthread_mutex_unlock(cq->mutex);
  _exit(1);
}
