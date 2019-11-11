/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h


int main(int argc, char *argv[]){
  if(argc<4){// 4 for no extra credits
    fprintf(stderr," Need more argument\n");
    return 1;
  }
  else if(argc == 4){
    FILE *fp;

    if((fp = fopen(argv[2],"r"))== NULL){
        fprintf(stderr, "%s is invalid\n",argv[2] );
        return 2;
    }else{
        filename=argv[2];
        
    }
  }
  else{
    fprintf(stderr,"Too much arguements\n");
    return 3;
  }


}
