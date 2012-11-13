#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

void move_to_front(unsigned char* string, int len, int actie){
  ascii_symbol *anker = (ascii_symbol*) malloc(sizeof(ascii_symbol)); //het anker element van onze linkedlist
  ascii_symbol *tijdelijk_anker = anker; //een symbool die we gebruiken om het anker tijdelijk in te bewaren
  ascii_symbol* backup_ascii_symbool; //een symbool die we gebruiken om tijdelijk
  anker->prev = NULL;
  anker->ascii_value = '\0';

  for(int i = 0; i <= 255; i++){
    anker->next = (struct ascii_symbol*) malloc(sizeof(ascii_symbol)); //allocate genoeg voor next symbool.
    backup_ascii_symbool = anker; //sla het huidige anker symbool op
    anker = (ascii_symbol*)anker->next; //sla het anker->next symbool op in anker
    anker->prev = (struct ascii_symbol*)backup_ascii_symbool; //anker->next->prev = anker
    anker->ascii_value = (unsigned char)i; //anker->next->value = i 
  }
  anker->next = tijdelijk_anker; //we laten het next element van het laatste element terug wijzen naar het begin.
  anker = tijdelijk_anker;
  
#ifdef MTF_DEBUG
  for(int i = 0; i <= 255; i++){
    anker = anker->next;
    printf("%d %c \n", i, anker->ascii_value);
  }
  anker = tijdelijk_anker;
#endif
  if(actie){
    for(int i = 0; i < len; i++){
      unsigned char c = 0;
      tijdelijk_anker = anker; //terug bij start.
      while((unsigned char)tijdelijk_anker->ascii_value != (unsigned char)string[i]){
	tijdelijk_anker = (ascii_symbol*)tijdelijk_anker->next;
	c++;
      }
      
      string[i] = c-1; //overschrijf origineel
      if(c == 0){
#ifdef debug
	//no point, we should not rehang the first element in our linkedlist
	printf("hey i just met you, and this is CRAZY!!.. so we should breakup.\n");
#endif
	break; //up
      }
      
      backup_ascii_symbool = (ascii_symbol*) tijdelijk_anker->prev;
      backup_ascii_symbool->next = tijdelijk_anker->next;
      
      backup_ascii_symbool = (ascii_symbol*) backup_ascii_symbool->next;
      backup_ascii_symbool->prev = (struct ascii_symbol*) tijdelijk_anker->prev;
      
      backup_ascii_symbool = (ascii_symbol*) anker->next;
      backup_ascii_symbool->prev = (struct ascii_symbol*) tijdelijk_anker;
      
      tijdelijk_anker->next = anker->next;
      tijdelijk_anker->prev = (struct ascii_symbol*)anker;
      anker->next = (struct ascii_symbol*)tijdelijk_anker;
      
    }
  }else{
#ifdef debug
    printf("start van move to front\n");
    printf("input string: %s met een lengte: %d\n", string,len);
    for(int i = 0; i < len; i++){
      printf("%d \n", string[i]);
    }
#endif
    for(int i = 0; i < len; i++){
      tijdelijk_anker = anker; //terug bij begin
      int j = 0;
      while(j != (int)string[i]){ 
	//	printf("hier: %d met symbool: %d\n",j,string[i]);
	tijdelijk_anker = (ascii_symbol*) tijdelijk_anker->next;
	
	j++;
      }

      tijdelijk_anker = (ascii_symbol*)tijdelijk_anker->next;
#ifdef debug
      printf("%d - %c \n",j, tijdelijk_anker->ascii_value);
#endif
      string[i] = tijdelijk_anker->ascii_value;
      //      printf("net overschreden value is: %d \n", string[i]);
      if(j == 255){
	anker = tijdelijk_anker; //zet een nieuw begin
	string[i]--;
	break;
      }
      
      if(tijdelijk_anker->prev != NULL){
	backup_ascii_symbool = (ascii_symbol*)tijdelijk_anker->prev;
      }else{
	backup_ascii_symbool = tijdelijk_anker;
      }
      
      backup_ascii_symbool->next = tijdelijk_anker->next;
      backup_ascii_symbool = (ascii_symbol*)backup_ascii_symbool->next;


      if(backup_ascii_symbool->prev != NULL){
	backup_ascii_symbool->prev = tijdelijk_anker->prev;
      }
      
      backup_ascii_symbool = (ascii_symbol*)anker->next;
      if(backup_ascii_symbool->prev != NULL)
	backup_ascii_symbool->prev = (struct ascii_symbol*)tijdelijk_anker;
      
      tijdelijk_anker->next = anker->next;
      if(tijdelijk_anker->prev != NULL)
	tijdelijk_anker->prev = (struct ascii_symbol*)anker;
      anker->next = (struct ascii_symbol*)tijdelijk_anker;
    }
  }
}
