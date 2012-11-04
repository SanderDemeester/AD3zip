#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>

void standaard_huffman(char *input_buffer, int lengte, int actie){
  char *freq_tabel = (char*) calloc(255,1);

  if(actie){
    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    for(int i = 0; i < lengte; i++){
      printf("%c \n", input_buffer[i]);
    }
  }else{
    //decodeer
  }
  
  free(freq_tabel);
}
