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
  pthread_mutex_lock(cq->mutex);
  FILE *fp;
  char line[1024];
  if((fp = fopen(filename,"r")) != NULL){
    q = cq->q;
    if(fgets(line,1024,fp)!=NULL){
      q->vals=line;
      q=q->next;
    }
    fclose(fp);
  }else{
      printf("fail to read file %s\n",argv[1]);
  }

  pthread_cond_signal(cq->cond);

  pthread_mutex_unlock(cq->mutex);
}
