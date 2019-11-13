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
  struct buffer* bq = (struct buffer*) malloc(sizeof(struct buffer));
  //struct buffer* q = cq->q;
  char newline[1025];

  FILE *fp;
  char line[1024];
  int lineNum =0;
  //endp =FALSE;
  if(strcmp(option,"-p")==0){
    fputs("producer\n",logfile);
  }
  if((fp = fopen(cq->filename,"r")) != NULL){
    bq = cq->q;
    while(fgets(line,1024,fp)!=NULL){
      pthread_mutex_lock(cq->mutex);
      bq -> next = (struct buffer*) malloc(sizeof(struct buffer));
      bq->next->check=0;
      sprintf(newline,"-%s",line);
      strcpy(bq->vals,newline);
      bq->check=1;//produced
      bq->lineNum=lineNum;
      bq=bq->next;//move to the next
      if(strcmp(option,"-p")==0){
        fprintf(logfile, "producer: %d\n",lineNum);
      }
      lineNum++;
      line[0]='\0';
      pthread_cond_signal(cq->cond);
      pthread_mutex_unlock(cq->mutex);
    }
    end=lineNum;
    //endp=TRUE;
    fclose(fp);
  }else{
      printf("fail to read file %s\n",cq->filename);
  }
  pthread_exit(NULL);
}
