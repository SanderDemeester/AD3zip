#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _GENERIC
#include "header/generic.h"
#include "header/compressie_methode.h"
#endif
#define DECODEER 0
#define ENCODEER 1

int main(int argc, char* argv[]){
  int methode = 0; //default methode is decodeer
  int blocksize = 0;
  char *input_buffer = (char*) malloc(sizeof(char));
  char *t = NULL;
  char c = '\0';
  int input_lengte = 0; 
  compressie_argument **compressie_methode = (compressie_argument**) malloc(sizeof(compressie_argument*)*4);

  for(int i = 0 ;i < 4; i++){
    compressie_methode[i] = (compressie_argument*) malloc(sizeof(compressie_argument));
  }
  compressie_methode[0]->value = 1;
  compressie_methode[0]->compressie_algoritme = mtf_huffman;

  compressie_methode[1]->value = 2;
  compressie_methode[1]->compressie_algoritme = not_implemented;

  compressie_methode[2]->value = 3;
  compressie_methode[2]->compressie_algoritme = semi_mtf;

  compressie_methode[3]->value = 4;
  compressie_methode[3]->compressie_algoritme = not_implemented;


  
  
  if((argc < 4 && ((argc == 2 && strcmp(argv[1],"decodeer"))) || argc==1)){
    fprintf(stderr, "Usage: %s [encodeer|decodeer] compressiemethode blocksize", argv[0]);
    exit(-1);
  }

  if(!strcmp(argv[1],"encodeer")) methode = ENCODEER;
  if(!strcmp(argv[1],"decodeer")) methode = DECODEER;
  
  if(methode){
    for(int i = 0; i < 4; i++){
      if(compressie_methode[i]->value == atoi(argv[2])) compressie_methode[i]->compressie_algoritme(0,0);
    }
    blocksize = atoi(argv[3]);
  }
  
  while((fread(&c,1,1,stdin))) {
    input_lengte++;
    t = (char*) realloc(input_buffer,input_lengte*sizeof(char));
    if(t != NULL){
      input_buffer = t;
      input_buffer[input_lengte-1] = c;
    }else{
      free(input_buffer);
      fprintf(stderr,"Fout tijdens allocteren van input buffer");
      exit(-1);
    }
  }
  if(methode){ //encodeer
    char *input_block = NULL; 
  /***************************************************************/
  /* input_buffer is de buffer die input tekens van stdin bevat	 */
  /* input_lengte is het aantal tekens dat input_buffer bevat	 */
  /* input_block  is de buffer die de tekens van de buffer bevat */
  /***************************************************************/
    input_lengte--; //remove the newline

#ifdef DEBUG
    printf("Encodeer\n");
    printf("%d \n", input_lengte);
#endif
    fwrite(&blocksize, 1, sizeof(blocksize)/sizeof(int), stdout);
    //    printf("\n");
    while(input_lengte){
      if(input_lengte < blocksize){
	blocksize = input_lengte;
	#ifdef DEBUG
	printf("%d \n", blocksize);
	#endif
      }
      input_block = (char*) malloc(sizeof(char)*blocksize);
      memcpy((void*)input_block, (void*) input_buffer, blocksize);
      encoderen_bwt(input_block, blocksize);
      input_lengte -= blocksize; //subtract from input_lengte
      input_buffer += blocksize; //add to input_buffer
      for(int i = 0; i < blocksize+2; i++){
	fwrite(&input_block[i], 1, sizeof(input_block[i]), stdout);
      }
    }
  }else{
#ifdef DEBUG
    printf("Decodeer\n");
#endif
    //Working buffer, in deze buffer komen de substrings
    char *input_block = NULL;
    
    //We verwijderen de extra '\n' (niet meer nodig)
    //    input_buffer[input_lengte-1] = '\0'; 
    
    //Vind de eerste substring
    //p = strchr(input_buffer, newline);
    
    //blocksize
    int blocksize = input_buffer[0];
    input_buffer++;

    while(input_lengte > 0){
      if(input_lengte < blocksize){
	blocksize = input_lengte;
      }
      input_block = (char*) malloc(sizeof(char)*blocksize+2);
      memcpy((void*)input_block, (void*) input_buffer, blocksize+2);
      input_lengte -= blocksize+2; //substract from input_lengte;
      input_buffer += blocksize+2; //add to input_buffer
      //printf("%s \n", input_block);
      decoderen_bwt(input_block, blocksize);
      printf("%s", input_block+2);
    }

/*************************************************************************/
/*     //Zolang er nog newlines mogen we doorgaan			 */
/*     while(p){							 */
/* #ifdef decode_debug							 */
/*       printf("de gevonden index: %d \n", p - input_buffer);		 */
/* #endif								 */
/*       working_buffer = (char*) malloc(sizeof(char)*(p-input_buffer)); */
/*       memcpy(working_buffer, input_buffer, p-input_buffer);		 */
/* #ifdef decode_debug							 */
/*       printf("de working buffer: %s \n", working_buffer);		 */
/* #endif								 */
/*       decoderen_bwt(working_buffer, (p-input_buffer)-2);		 */
/*       input_buffer += (p-input_buffer)+1;				 */
/* #ifdef decode_debug							 */
/*       printf("de aangepaste input_buffer: %s", input_buffer);	 */
/* #endif								 */
/*       p = strchr(input_buffer, newline);				 */
/* #ifdef decode_debug							 */
/*       printf("de nieuwe gevonden index: %d \n", p - input_buffer);	 */
/* #endif								 */
/*       free(working_buffer);						 */
/*     }								 */
/*************************************************************************/
  }
  free(t);
  return 0;
}
