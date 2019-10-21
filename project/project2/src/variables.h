#ifndef __VARIABLES_H
#define __VARIABLES_H

#define LINE_SIZE 1000
#define MAX_LINES 10000
#define SIZE_TXTPATH 100
#define ALPHA_NUM_SIZE 27

char lines[MAX_LINES][LINE_SIZE];
int count ;

typedef struct mapper {
    char * val;
    struct node * next;
} mapper_t;
#endif
