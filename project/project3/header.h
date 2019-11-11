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

// header here..

struct buffer {
   char vals[1024];
   struct buffer *next;
}*Buffer;

struct condBuffer {

	struct buffer* q;
  pthread_cond_t* cond;
	pthread_mutex_t* mutex;
};

extern char * filename;
extern FILE * logfile;
// struct queue {
//      struct thread_s *head;
//      pthread_cond_t* cond;
//      pthread_mutex_t* mutex;
// };

// struct thread_s{
//     char c;
//     struct thread_s *next;
// };
#endif
