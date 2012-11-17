#include "header/compressie_methode.h"
#include "header/move_to_front.h"
#include "header/huffman.h"
#include <stdio.h>

/************************************************************************/
/* Elke van deze functies delegeert het werk naar het juiste algoritme. */
/* Deze functies worden bepaalde door een functie pointer.	        */
/************************************************************************/
void not_implemented(unsigned char *input_buffer, int len, int actie){
  printf("Not implemented, options 2 and 4.\n");
}

void mtf_huffman(unsigned char *input_buffer, int len, int actie){
  #ifdef DEBUG
  printf("Move to Front in combinatie met standaard huffman, optie 1\n");
  #endif
  if(actie){
    //Tijdens encoderen eerst move to front, daarna stadnaard huffman
    move_to_front(input_buffer, len, actie);  
    standaard_huffman(input_buffer, len, actie);
  }else{
    //Tijdens het decoderen doen we net het omgekeerde
    standaard_huffman(input_buffer,len,actie);    
  }
}

void semi_mtf(unsigned char *input_buffer, int len, int actie){
  printf("Semi move to front in combinatie met standaard huffman, optie 3\n");
}

void debug(unsigned char *input_buffer, int len, int actie){
  fwrite(input_buffer, 1,len,stdout);
}
