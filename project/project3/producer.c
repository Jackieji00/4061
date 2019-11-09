/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h


void *producer(void *arg){
    FILE *fp;
    if((fp = fopen(argv[1],"r")) != NULL){
    int lines = 0;
    int c =fgect(fp);
    while (c != EOF) {
        if (c == '\n') {
            lines++;
        }
        lines = lines +1;
      }
    }else{
        printf("fail to read file %s\n",argv[1]);
    }
}
