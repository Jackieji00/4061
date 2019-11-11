/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

// implement shared queue, final histogram here..
 void finilize(){
   FILE * fp = fopen("result.txt", "w");
   if(fp == NULL){
     printf("Unable to create file.\n");
     exit(EXIT_FAILURE);
   }else{
       int c = 0;
       for(int i = 0;i<26;i++){
         c=i+65;
         char alphabet = c;
         fprintf(fp,"%c: %d\n",alphabet,alphaCount[i]);
       }
     }
     fclose(fp);
     return;
 }
