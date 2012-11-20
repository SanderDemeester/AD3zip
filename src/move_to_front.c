#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

#ifdef MTF_DEBUG
static void debug(ascii_symbol* anker){
  ascii_symbol *echt = anker->next;
  printf("%d \n",echt->ascii_value);
  for(int i = 0; i <= 255; i++){
    
    ascii_symbol *prev = echt->prev;
    ascii_symbol *next = echt->next;
    if(prev == NULL){
      printf("prev is null\n");
    }
    if(next == NULL)
      printf("prev is null\n");
    printf("huidige index nummer: %d && prev: %d && next: %d && huidig: %d \n", i, prev->ascii_value, next->ascii_value, echt->ascii_value);
    echt =  next;
  }
  printf("done\n");
}
#endif

void move_to_front(unsigned char* string, int len, int actie){
  ascii_symbol *anker = (ascii_symbol*) malloc(sizeof(ascii_symbol)); //het anker element van onze linkedlist
  ascii_symbol *tijdelijk_anker = anker; //een symbool die we gebruiken om het anker tijdelijk in te bewaren
  ascii_symbol* backup_ascii_symbool; //een symbool die we gebruiken om tijdelijk
  ascii_symbol *first = NULL;
  anker->prev = NULL;
  anker->ascii_value = '\0';
  anker->next = (struct ascii_symbol*) malloc(sizeof(ascii_symbol));
  anker = (ascii_symbol*)anker->next; //wijst naar eerste echte element
  first = anker; //eerste echte element
  anker->ascii_value = (unsigned char)0;
  
  for(int i = 1; i <= 255; i++){
    anker->next = (struct ascii_symbol*) malloc(sizeof(ascii_symbol)); //allocate genoeg voor next symbool.
    backup_ascii_symbool = anker; //sla het huidige anker symbool op
    anker = (ascii_symbol*)anker->next; //sla het anker->next symbool op in anker
    anker->prev = (struct ascii_symbol*)backup_ascii_symbool; //anker->next->prev = anker
    anker->ascii_value = (unsigned char)i; //anker->next->value = i 
  }
  //op dit momoment is anker het laatste echte element.
  anker->next = (struct ascii_symbol*) first; //we linken gewoon terug naar het eerste echte element.
  first->prev = (struct ascii_symbol*) anker;
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
      #ifdef MTF_DEBUG
      debug(anker); 
      #endif
      unsigned char c = 0;
      tijdelijk_anker = (ascii_symbol*) anker->next; //terug bij start.
      while((unsigned char)tijdelijk_anker->ascii_value != (unsigned char)string[i]){
	tijdelijk_anker = (ascii_symbol*)tijdelijk_anker->next;
	#ifdef MTF_DEBUG
	printf("%d && %d \n", tijdelijk_anker->ascii_value,string[i]);
	#endif
	c++;
      }     

      #ifdef MTF_DEBUG
      printf("c:%d \n");
      #endif

      string[i] = c; //overschrijf origineel
      if(c == 255){
	first = (ascii_symbol*) anker->next; //oude first element
	tijdelijk_anker->next = (struct ascii_symbol*) first;
	anker->next = (struct ascii_symbol*) tijdelijk_anker;
      }else if(c > 0){
	backup_ascii_symbool = (ascii_symbol*) tijdelijk_anker->prev;
	backup_ascii_symbool->next = tijdelijk_anker->next;
	
	backup_ascii_symbool = (ascii_symbol*) backup_ascii_symbool->next;
	backup_ascii_symbool->prev = (struct ascii_symbol*) tijdelijk_anker->prev;
	
	//op dit moment unlinkt van de ketting
	
	
	first = (ascii_symbol*) anker->next;
	tijdelijk_anker->prev = (struct ascii_symbol*)first->prev;
	first->prev = (struct ascii_symbol*) tijdelijk_anker;
	
	tijdelijk_anker->next = (struct ascii_symbol*) first;
	
	anker->next = (struct ascii_symbol*)tijdelijk_anker;
      }
    }
  }else{
    for(int i = 0; i < len; i++){
      tijdelijk_anker = (ascii_symbol*) anker->next; //terug linken naar begin
      int j = 0;
      while(j != (int)string[i]){
	tijdelijk_anker = (ascii_symbol*)tijdelijk_anker->next;
	j++;
      }
      //tijdelijk_anker = (ascii_symbol*) tijdelijk_anker->next;
      
      string[i] = tijdelijk_anker->ascii_value;
      if(j > 0){
	backup_ascii_symbool = (ascii_symbol*) tijdelijk_anker->prev;
      backup_ascii_symbool->next = tijdelijk_anker->next;
      
      backup_ascii_symbool = (ascii_symbol*) backup_ascii_symbool->next;
      backup_ascii_symbool->prev = (struct ascii_symbol*) tijdelijk_anker->prev;

      //op dit moment unlinkt van de ketting
      
      first = (ascii_symbol*) anker->next;
      tijdelijk_anker->prev = (struct ascii_symbol*)first->prev;
      first->prev = (struct ascii_symbol*) tijdelijk_anker;
      
      tijdelijk_anker->next = (struct ascii_symbol*) first;

      anker->next = (struct ascii_symbol*)tijdelijk_anker;
      }else if(j==255){
	anker->next = (struct ascii_symbol*) tijdelijk_anker;
      }
    }
  }
  ascii_symbol *n = (ascii_symbol*)anker->next;
  ascii_symbol *f = (ascii_symbol*)anker->next;
  for(int i = 0; i <= 255; i++){
    n = (ascii_symbol*)f->next;
    free(f);
    f = n;
  }
  free(anker);
}


