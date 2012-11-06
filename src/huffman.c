#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>

/**********************************************************************************************************************************************/
/* We weten dat de kans groter is dat de symbolen met een hogeren freqentie verder zullen staan in de input_buffer->symbolen later in de rij  */
/* zullen hogere freqentie hebben													      */
/**********************************************************************************************************************************************/
static void swap(int *a, int *b){
  int t = *a; 
  *a = *b;
  *b = t;
}
int static ssort(const void *x, const void *y) {
  
}

void standaard_huffman(char *input_buffer, int lengte, int actie){
  int *freq_tabel = (int*) calloc(255,4);
  huffman_blad **huffman_toppen = (huffman_blad**) calloc(lengte,sizeof(huffman_blad*));

  if(actie){
    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    for(int i = 0; i < lengte; i++){
      printf("%c \n", input_buffer[i]);
      freq_tabel[(int)input_buffer[i]]++;
    }
    qsort(freq_tabel, 255, sizeof(int), sort);
    for(int i = 0; i < 255; i++){
      printf("index: %i - value: %d \n", i, freq_tabel);
    }
  }else{
    //decodeer
  }
  for(int i = 0; i < lengte; i++){
    //    free(huffman_toppen[i]->value);
    free(huffman_toppen[i]);
  }
  
  free(freq_tabel);
  free(huffman_toppen);
}
