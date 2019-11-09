/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

struct thread_s{
    char c;
    struct thread_s *next;
};

//struct buffer {
//    int vals[100];
//    int index; //indicates the next available spot
//};

struct queue {
     struct thread_s *head;
     pthread_cond_t* cond;
     pthread_mutex_t* mutex;
};

int main(int argc, char *argv[]){
    if(argc<4){// 4 for no extra credits
        fprintf(stderr," Need more argument\n");
        return 1;
    }
    else if(argc == 4){
        DIR *dr;
        if((dr=opendir(argv[1]))==NULL){
            fprintf(stderr, "%s is invalid\n",argv[1] );
            return 2;
//            else{
//                FILE * fp = fopen(argv[1],"r");//Read the file
//                int lines;
//            }
        }
    }
    else{
        fprintf(stderr,"Too much arguements\n");
        return 3;
    }


    
    
    


}
