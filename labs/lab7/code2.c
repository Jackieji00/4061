#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct node{
  int *x;
  struct node* next;

};

struct node* create_node(int i){
  struct node temp;
  *(temp.x)=i;
  return temp;

}

int main() {
    int i=0;
    struct node* first;
    struct node* prev ;
    struct node* temp;

    first=create_node(i);


    prev=first;
    while(i<10){
      i++;
      temp=create_node(i);
      prev->next=temp;
      prev=temp;
    }
    free(first);
    free(temp);

    return 0;
}
