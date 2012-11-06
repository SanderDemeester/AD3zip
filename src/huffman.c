#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>

/**********************************************************************************************************************************************/
/* We weten dat de kans groter is dat de symbolen met een hogeren freqentie verder zullen staan in de input_buffer->symbolen later in de rij  */
/* zullen hogere freqentie hebben													      */
/**********************************************************************************************************************************************/

void standaard_huffman(char *input_buffer, int lengte, int actie){
  char *freq_tabel = (char*) calloc(255,1);
  huffman_blad **huffman_toppen = (huffman_blad**) calloc(lengte,sizeof(huffman_blad*));

  if(actie){
    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    for(int i = 0; i < lengte; i++) freq_tabel[(int)input_buffer[i]]++;
    for(int i = 0; i < 255; i++){
      if(freq_tabel[i] > 0){
	huffman_toppen[i] = (huffman_blad*) calloc(1,sizeof(huffman_blad));
	printf("symbool: %c, relatieve freq: %d \n", i, freq_tabel[i]);
      }
    }
  }else{
    //decodeer
  }
  for(int i = 0; i < lengte; i++) free(huffman_toppen[i]);
  
  free(freq_tabel);
  free(huffman_toppen);
}
