#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _GENERIC
#include "header/generic.h"
#include "header/compressie_methode.h"
#include "header/bw.h"
#endif
#define DECODEER 0
#define ENCODEER 1
#define NUMBER_OF_COMPRESSIE_METHODE 5 //+1 for debug mode.

int main(int argc, char* argv[]){

  int methode = 0; //default methode is decodeer
  int compressie_function_pointer = 0; //default is debug.
  int blocksize = 0; //de blocksize die wordt gebruikt, dit is de lengte zonder extra header
  int aantal_ingelezen_bytes = 0; //het aantal bytes dat zijn ingelezen door freader
  unsigned char *input_buffer = (unsigned char*) malloc(sizeof(unsigned char)); //de buffer die fread gebruikt om zijn data in op te slaan.
  unsigned char* header = (unsigned char*) calloc(4,sizeof(unsigned char)); //4 byte for header
  unsigned char methode_header_format = '\0'; //we will use this to copy our methode identifier
  unsigned char *t = NULL; //Tijdelijke pointer voor freader
  unsigned char c = '\0'; //hierin laadt fread een byte
  unsigned char *input_block = NULL;
  int input_lengte = 0; 
  compressie_argument **compressie_methode = (compressie_argument**) malloc(sizeof(compressie_argument*)*NUMBER_OF_COMPRESSIE_METHODE);

  /**********************************************************************************************************************************************************************************************************************/
  /* We maken gebruik van een array van structs die een id bevatten per compressie methode en een function pointer naar de bijhorende compressie methode. We gebruiken methode 0 om enkel Burrows-wheeler te gebruiken  */
  /**********************************************************************************************************************************************************************************************************************/
  
  for(int i = 0 ;i < NUMBER_OF_COMPRESSIE_METHODE; i++){
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
  
  compressie_methode[4]->value = 0; //debug
  compressie_methode[4]->compressie_algoritme = debug;
  
  if(argc == 1 || (argc < 4 && strcmp(argv[1],"decodeer"))){
    fprintf(stderr, "Usage: %s [encodeer|decodeer] compressiemethode blocksize", argv[0]);
    exit(-1);
  }

  if(!strcmp(argv[1],"encodeer")) methode = ENCODEER;
  if(!strcmp(argv[1],"decodeer")) methode = DECODEER;
  
  if(methode == ENCODEER){
    for(int i = 0; i < NUMBER_OF_COMPRESSIE_METHODE; i++){
      if(compressie_methode[i]->value == atoi(argv[2])) compressie_function_pointer = i;
    }
    blocksize = atoi(argv[3]);
  }else{
    compressie_function_pointer = 4; //Debug methode.
  }
  if(methode == DECODEER){
    fread(header,4,1,stdin); //header is 4bytes
  }
  
  while((fread(&c,1,1,stdin))) {
    input_lengte++;
    t = (unsigned char*) realloc(input_buffer,input_lengte*sizeof(unsigned char));
    if(t != NULL){
      input_buffer = t;
      input_buffer[input_lengte-1] = c;
    }else{
      free(input_buffer);
      fprintf(stderr,"Fout tijdens allocteren van input buffer");
      exit(-1);
    }
  }
  if(methode == ENCODEER){ //encodeer
    
  /***************************************************************/
  /* input_buffer is de buffer die input tekens van stdin bevat	 */
  /* input_lengte is het aantal tekens dat input_buffer bevat	 */
  /* input_block  is de buffer die de tekens van de buffer bevat */
  /***************************************************************/

#ifdef DEBUG
    printf("Encodeer\n");
    printf("%d \n", input_lengte);
#endif
    

    blocksize = (blocksize & 0x00FFFFFF);
    methode_header_format = (unsigned char) ((compressie_function_pointer & 0x000000FF));

    memcpy(&header[0],&methode_header_format,1);
    memcpy(&header[1],&blocksize,3);
    
    fwrite(&header[0],1,4,stdout); //schrijf header naar stdout.

    while(input_lengte){
      if(input_lengte < blocksize){
	blocksize = input_lengte;
#ifdef DEBUG
	printf("aanpasen blocksize: %d \n", blocksize);
#endif
      }
      
      input_block = (unsigned char*) malloc(sizeof(unsigned char)*(blocksize+5)); //alocte genoeg om de blok in op te slaan +5 voor een int en "_"
      memcpy((void*)input_block, (void*) input_buffer, blocksize); //laat 5 plaatsen over.

      encoderen_bwt(input_block, blocksize);
      input_lengte -= blocksize; //subtract from input_lengte
      input_buffer += blocksize; //add to input_buffer

      aantal_ingelezen_bytes += blocksize;
      compressie_methode[compressie_function_pointer]->compressie_algoritme(input_block,blocksize+5,ENCODEER); //+5 voor 1 extra int en een _ teken.
      //free(input_block);
    }

    input_buffer -= aantal_ingelezen_bytes;
    free(input_buffer);
  }else{
#ifdef DEBUG
    printf("Decodeer\n");
#endif
    //Methode is de eerste byte van de header
    compressie_function_pointer = header[0];
    
    //De volgende 3 bytes zijn de blocksize
    memcpy(&blocksize,&header[1],3);
    
    //layout: 4byte (bwt index)|blocksize byte: bwt vector

    /********************/
    /* input_buffer+=4; */
    /* input_lengte-=4; */
    /********************/
    //Zolang er nog input is 
    while(input_lengte > 0){
      //Als algiment niet klopt, pas dit aan.

      if(input_lengte < blocksize+5){
	/********************************************************************************************************/
        /* We gebruiken hier -5 omdat er rekening wordt gehouden met de 2 controle chars per deelstring         */
	/* bij het decoderen van de bwt vector							                */
        /********************************************************************************************************/
	blocksize = input_lengte-5; 
      }
      
      input_block = (unsigned char*) malloc(sizeof(unsigned char)*blocksize+5);
      memcpy((void*)input_block, (void*) input_buffer, blocksize+5);
      input_lengte -= blocksize+5; //substract from input_lengte;
      input_buffer += blocksize+5; //add to input_buffer
      
      /******************************************************************************/
      /* Met deze controle kijken we of onze input enkel maar de bwt is.		  */
      /*   Deze controle is tijdelijk, en enkel maar nodig voor tijdens devlopement */
      /******************************************************************************/
      if(compressie_function_pointer < 4){ 
	compressie_methode[compressie_function_pointer]->compressie_algoritme(input_block,blocksize+5, DECODEER);
      } 
      decoderen_bwt(input_block, blocksize);
      input_block += 5;
      
      //We moeten altijd van onze bwt terug naar normale tekst, dit is niet afhankelijk van de compressie methode.
      compressie_methode[4]->compressie_algoritme(input_block,blocksize, methode);
      input_block-=5;
      free(input_block);
    }
  }

  for(int i = 0 ;i < NUMBER_OF_COMPRESSIE_METHODE; i++){
    free(compressie_methode[i]);
  }

  free(compressie_methode);
  return 0;
  
}
