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
    // printf("%c\n",c);
    c=line[0];
    if(isalpha(c)==0 && isalpha(line[1])!=0){
      // printf("is:%c%c\n",c,line[1]);
      if(line[1]<97){
        alphaCount[line[1]-65]++;
      }else{
        alphaCount[line[1]-97]++;
      }
    }
    line++;
  }
}

void * consumer(void* arg){

  struct condBuffer* cq = (struct condBuffer*) arg;
  struct buffer* q = (struct buffer*) malloc(sizeof(struct buffer));
  int lineNum=0;
  int customerID= cq->consumerId;
  if(strcmp(option,"-p")==0){
    fprintf(logfile, "consumer %d\n",cq->consumerId);
  }
  q=cq->q;
  while(end>lineNum||end==0){
    pthread_mutex_lock(cq->mutex);
    //pthread_cond_signal(cq->cond);
    while(q->check==0){
        pthread_cond_wait(cq->cond, cq->mutex);
    }
    if(q->check==2){
      q=q->next;
    }else if(q->check==1){
      q->check=2;
      wordcount(q->vals);
      if(strcmp(option,"-p")==0){
        fprintf(logfile, "consumer %d: %d\n",customerID,lineNum);
      }
      q=q->next;

    }
    lineNum++;
    pthread_cond_signal(cq->cond);
    pthread_mutex_unlock(cq->mutex);
  }
  pthread_exit(NULL);
}
