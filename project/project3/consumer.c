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
  size_t i =0;
  int c;
//read the text file manually add the 1st word, then add every alphabet after the whitespace,
//newline, tab, etc. store the result in the variable alphaCount
  c=line[i++];
  if(isalpha(c)!=0){
    if(c<97){
      alphaCount[c-65]++;
    }else{
      alphaCount[c-97]++;
    }
  }
  while(line[i]!='/0'){
    c=line[i++];
    if(!isalpha(c)){
      c=line[i++];
      if(isalpha(c)!=0){
        if(c<97){
          alphaCount[c-65]++;
        }else{
          alphaCount[c-97]++;
        }
      }
    }
  }
}

void consumer(void* arg){

	// struct Buffer* cq = (struct condBuffer*) arg;
  // pthread_mutex_lock(cq->mutex);
  // while(cq->q->index==0){
  //     pthread_cond_wait(cq->cond, cq->mutex);
  // }
  //
  // wordcount(cq)
  //
  //   pthread_mutex_unlock(cq->mutex);
}
