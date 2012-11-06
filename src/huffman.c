#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************************************************************************************/
/* We weten dat de kans groter is dat de symbolen met een hogeren freqentie verder zullen staan in de input_buffer->symbolen later in de rij  */
/* zullen hogere freqentie hebben													      */
/**********************************************************************************************************************************************/
static void swap(int *a, int *b){
  int t = *a; 
  *a = *b;
  *b = t;
}
//Als je iets goed wilt (of gedaan op een manier dat je wilt snappen) doe het zelf.
static void ssort(int *rij, int begin, int einde){
  if(einde > begin+1){
    int piv = rij[begin]; //piv is eerste element van rij
    int links = begin+1;
    int rechts = einde;
    while(links < rechts){
      if(rij[links] >= piv) links++;
      else swap(&rij[links],&rij[--rechts]);
    }
    swap(&rij[--links],&rij[begin]);
    ssort(rij,begin,links);
    ssort(rij,rechts,einde);
  }
}

void standaard_huffman(char *input_buffer, int lengte, int actie){
  int *freq_tabel = (int*) calloc(255,4);
  huffman_blad **huffman_toppen = (huffman_blad**) calloc(lengte,sizeof(huffman_blad*));
  int j = 0;
  if(actie){
    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    for(int i = 0; i < lengte; i++){
      printf("%c \n", input_buffer[i]);
      freq_tabel[(int)input_buffer[i]]++;
    }
    for(int i = 0; i < 255; i++){
      if(freq_tabel[i] > 0){
	huffman_toppen[j] = (huffman_blad*) calloc(1,sizeof(huffman_blad));
	huffman_toppen[j]->value = (char*) calloc(1,sizeof(char));
	memcpy(huffman_toppen[j]->value,&i,4);
	huffman_toppen[j]->weight = freq_tabel[i];
	j++;
      }
    }
    ssort(freq_tabel, 0,255);
    for(int i = 0; i < 255; i++){
      printf("index: %i - value: %d \n", i, freq_tabel[i]);
    }
  }else{
    //decodeer
  }
  for(int i = 0; i < lengte; i++){
    free(huffman_toppen[i]->value);
    free(huffman_toppen[i]);
  }
  free(freq_tabel);
  free(huffman_toppen);
}
