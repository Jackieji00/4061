/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

void wordcount(char * line){
  FILE * fpTxt;
  int c;
//read the text file manually add the 1st word, then add every alphabet after the whitespace,
//newline, tab, etc. store the result in the variable alphaCount
  while(line[0]!='\0'){
    c=line[0];
    if(isalpha(c)!=0 && isalpha(line[1])==0){
      if(c<97){
        alphaCount[c-65]++;
      }else{
        alphaCount[c-97]++;
      }
    }
    line++;
  }
}

void * consumer(void* arg){
  FILE * logfile;
  struct condBuffer* cq = (struct condBuffer*) arg;
  struct buffer* q = (struct buffer*) malloc(sizeof(struct buffer));
  int lineNum=0;
  if(strcmp(option,"-p")==0){
    logfile=fopen("log.txt","a");
    fprintf(logfile, "consumer %d\n",cq->consumerId);
  }
  pthread_mutex_lock(cq->mutex);
  while(cq->q->check==0){
      pthread_cond_wait(cq->cond, cq->mutex);
  }//why we need wait, if the prodcuer is working it will lock the queue
  q=cq->q;
  while(end!=TRUE){
    if(q->check==0){
        pthread_cond_wait(cq->cond, cq->mutex);
    }else if(q->check==2){
      q=q->next;
      lineNum++;
    }
  }
  if(q->check==1){//already produced
    wordcount(q->vals);
    fprintf(logfile, "consumer %d: %d\n",cq->consumerId,lineNum);
    fclose(logfile);
  }
  pthread_mutex_unlock(cq->mutex);
  fclose(logfile);
  _exit(1);
}
