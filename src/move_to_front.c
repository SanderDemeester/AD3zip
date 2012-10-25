#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

void move_to_front(char* string, int len){
  printf("begin move_to_front\n");
  ascii_symbol *first = (ascii_symbol*) malloc(sizeof(ascii_symbol));
  ascii_symbol *t = first;
  ascii_symbol* b;
  first->prev = NULL;
  first->ascii_value = '\0';
  
  for(int i = 0; i <= 255; i++){
    first->next = (ascii_symbol*) malloc(sizeof(ascii_symbol));
    b = first;
    first = first->next;
    first->prev = b;
    first->ascii_value = (unsigned char)i;
  }
  first = t;
  for(int i = 0; i <= 255; i++){
    first = first->next;
    printf("%d %c \n", i, first->ascii_value);
  }


  //start to free
  ascii_symbol *f = first;
  for(int i = 0; i <= 255; i++){
    t = first->next;
    free(f);
    f = t;
  }
  
  printf("end move to front\n");
}
