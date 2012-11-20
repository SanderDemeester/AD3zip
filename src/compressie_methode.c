#include "header/bw.h"
#include "header/compressie_methode.h"
#include "header/move_to_front.h"
#include "header/huffman.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif
#include <stdio.h>

/************************************************************************/
/* Elke van deze functies delegeert het werk naar het juiste algoritme. */
/* Deze functies worden bepaalde door een functie pointer.	        */
/************************************************************************/
void not_implemented(unsigned char *input_buffer, int len, int actie,int blocksize){
  printf("Not implemented, options 2 and 4.\n");
}

void mtf_huffman(unsigned char *input_buffer, int len, int actie,int blocksize){
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
    len = input_buffer[0];
    input_buffer++;
    //De eerste byte is de lengte van de gedecodeerde huffman code
    move_to_front(input_buffer, len, actie);
    fwrite(input_buffer, 1,len,stdout);
    input_buffer--;    
   }
}

void semi_mtf(unsigned char *input_buffer, int len, int actie,int blocksize){
  printf("Semi move to front in combinatie met standaard huffman, optie 3\n");
}

void debug(unsigned char *input_buffer, int len, int actie,int blocksize){
  if(actie){
    //Encoderen
    fwrite(input_buffer, 1,len,stdout);
  }else{
    //Decoderen
    unsigned char *input_block = NULL;
    while(len > 0){
      if(len < blocksize+5){
	blocksize = len-5;
      }

      input_block = (unsigned char*) malloc(sizeof(unsigned char)*blocksize+5);
      memcpy((void*)input_block, (void*) input_buffer, blocksize+5);
      len -= blocksize+5; //substract from input_lengte;
      input_buffer += blocksize+5; //add to input_buffer

      decoderen_bwt(input_block,blocksize);
      input_block += 5;
      free(input_block);
    }
  }
}
