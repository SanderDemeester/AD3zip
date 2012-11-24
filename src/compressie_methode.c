#include "header/bw.h"
#include "header/compressie_methode.h"
#include "header/move_to_front.h"
#include "header/huffman.h"
#include "header/lz77.h"
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

/* Move to front samen met standaard huffman */
void mtf_huffman(unsigned char *input_buffer, int len, int actie,int blocksize){

  if(actie){
    move_to_front(input_buffer, len, actie);  
    standaard_huffman(input_buffer, len, actie);
  }else{
    
    unsigned char *input_block = NULL; 
    uint32_t huffman_blocksize = 0;
    huffman_decode_result * huffman_result = NULL;

      while(len > 0){
	memcpy(&huffman_blocksize,input_buffer,4);     
	
	if(len < huffman_blocksize + HUFFMAN_HEADER_SIZE){
	  huffman_blocksize = len-HUFFMAN_HEADER_SIZE;
	}
	
	input_block = (unsigned char*) malloc(sizeof(unsigned char)*(huffman_blocksize+HUFFMAN_HEADER_SIZE));
	memcpy((void*) input_block, (void*) input_buffer, (huffman_blocksize+HUFFMAN_HEADER_SIZE));

	//Pas standaard huffman toe
	huffman_result = standaard_huffman(input_block,len,actie);    

	//pas move to front toe.
	move_to_front(huffman_result->res,huffman_result->aantal_bytes,actie);
	//len-BWT_HEADER_LEN-1 is omdat bwt header niet in rekening wordt gebracht tijdens decoderen van bwt.
	decoderen_bwt(huffman_result->res,huffman_result->aantal_bytes-BWT_HEADER_LEN);

	//print de bwt header niet mee.
	fwrite(huffman_result->res+BWT_HEADER_LEN,1,huffman_result->aantal_bytes-BWT_HEADER_LEN,stdout);
	
	len -= huffman_blocksize+HUFFMAN_HEADER_SIZE; //HUFFMAN_HEADER_SIZE bytes is the size of the huffman header
	
	input_buffer += huffman_blocksize+HUFFMAN_HEADER_SIZE; //HUFFMAN_HEADER_SIZE bytes is thesize of the huffman header	
	free(input_block);
	free(huffman_result->res);
	free(huffman_result);
      }            
  }
}

void semi_mtf(unsigned char *input_buffer, int len, int actie,int blocksize){
  printf("Semi move to front in combinatie met standaard huffman, optie 3\n");
}

void lz77(unsigned char* input_buffer, int len, int actie, int blocksize){
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
      
      //Decoderen van bwt
      decoderen_bwt(input_block,blocksize);
      input_block += 5;
      fwrite(input_block,1,blocksize,stdout);
      input_block -= 5;
      
      free(input_block);
    }
  }
}
