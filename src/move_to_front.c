#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

void move_to_front(char* string, int len){
  printf("begin move_to_front\n");
  ascii_symbol *anker = (ascii_symbol*) malloc(sizeof(ascii_symbol)); //het anker element van onze linkedlist
  ascii_symbol *tijdelijk_anker = anker;
  ascii_symbol* backup_ascii_symbool;
  anker->prev = NULL;
  anker->ascii_value = '\0';
  
  for(int i = 0; i <= 255; i++){
    anker->next = (ascii_symbol*) malloc(sizeof(ascii_symbol));
    backup_ascii_symbool = anker;
    anker = anker->next;
    anker->prev = backup_ascii_symbool;
    anker->ascii_value = (unsigned char)i;
  }
  anker = tijdelijk_anker;
  for(int i = 0; i <= 255; i++){
    anker = anker->next;
    printf("%d %c \n", i, anker->ascii_value);
  }


  //start to free
  ascii_symbol *element_to_free = anker;
  for(int i = 0; i <= 255; i++){
    tijdelijk_anker = anker->next;
    free(element_to_free);
    element_to_free = tijdelijk_anker;
  }
  
  printf("end move to front\n");
}
