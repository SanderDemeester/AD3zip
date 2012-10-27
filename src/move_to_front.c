#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

void move_to_front(char* string, int len, int actie){
  ascii_symbol *anker = (ascii_symbol*) malloc(sizeof(ascii_symbol)); //het anker element van onze linkedlist
  ascii_symbol *tijdelijk_anker = anker; //een symbool die we gebruiken om het anker tijdelijk in te bewaren
  ascii_symbol* backup_ascii_symbool; //een symbool die we gebruiken om tijdelijk
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
  
#ifdef MTF_DEBUG
  for(int i = 0; i <= 255; i++){
    anker = anker->next;
    printf("%d %c \n", i, anker->ascii_value);
  }
  anker = tijdelijk_anker;
#endif MTF_DEBUG
  
  if(actie){
    for(int i = 0; i < len; i++){
      char c = 0;
      tijdelijk_anker = anker; //terug bij start.
      while((unsigned char)tijdelijk_anker->ascii_value != (unsigned char)string[i]){
	tijdelijk_anker = tijdelijk_anker->next;
	c++;
      }
      string[i] = c-1; //overschrijf origineel
      backup_ascii_symbool = tijdelijk_anker->prev;
      backup_ascii_symbool->next = tijdelijk_anker->next;
      
      backup_ascii_symbool = backup_ascii_symbool->next;
      backup_ascii_symbool->prev = tijdelijk_anker->prev;
      
      backup_ascii_symbool = anker->next;
      backup_ascii_symbool->prev = tijdelijk_anker;
      
      tijdelijk_anker->next = anker->next;
      tijdelijk_anker->prev = anker;
      anker->next = tijdelijk_anker;
    }
  }else{
    for(int i = 0; i < len; i++){
      char c = 0;
      tijdelijk_anker = anker; //terug bij begin
      printf("%c \n", string[i]);
      while((unsigned char)tijdelijk_anker->ascii_value != (unsigned char)string[i]){
	tijdelijk_anker = tijdelijk_anker->next;
	c++;
      }
      
      if(tijdelijk_anker->prev != NULL)
	backup_ascii_symbool = tijdelijk_anker->prev;
      else
	backup_ascii_symbool = tijdelijk_anker;
      
      backup_ascii_symbool->next = tijdelijk_anker->next;
      
      backup_ascii_symbool = backup_ascii_symbool->next;
      if(backup_ascii_symbool->prev != NULL)
	backup_ascii_symbool->prev = tijdelijk_anker->prev;
      
      backup_ascii_symbool = anker->next;
      if(backup_ascii_symbool->prev != NULL)
	backup_ascii_symbool->prev = tijdelijk_anker;
      
      tijdelijk_anker->next = anker->next;
      if(tijdelijk_anker->prev != NULL)
	tijdelijk_anker->prev = anker;
      anker->next = tijdelijk_anker;

    }
  }
}
