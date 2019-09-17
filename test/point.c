#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int * p = (int*) malloc(sizeof(int));
  printf("*p:%d\n&p:%p",*p,&p);
  return 0;
}
