/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

/*
header.h, header for all source files
it will:
- structure definition
- global variable, lock declaration (extern)
- function declarations
*/

#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <ctype.h>

// header here..

struct buffer {
   char vals[1024];
   int check;//0 for initialize, 1 for produced,2 for consumed
   int lineNum;
   struct buffer *next;
};

struct condBuffer {

	struct buffer* q;
  int consumerId;
  char * filename;
  pthread_cond_t* cond;
	pthread_mutex_t* mutex;
};

 void finilize();
 void *consumer(void* arg);
 void *producer(void *arg);
// static const struct buffer EmptyStruct={0};
extern char * option;
extern int end;
extern int alphaCount[25];
// struct queue {
//      struct thread_s *head;
//      pthread_cond_t* cond;
//      pthread_mutex_t* mutex;
// };

// struct thread_s{
//     char c;
//     struct thread_s *next;
// };

#define FALSE 0
#define TRUE 1
#endif
