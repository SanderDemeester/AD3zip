#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************************************************************************************/
/* We weten dat de kans groter is dat de symbolen met een hogeren freqentie verder zullen staan in de input_buffer->symbolen later in de rij  */
/* zullen hogere freqentie hebben													      */
/**********************************************************************************************************************************************/
static void swap(huffman_top *a, huffman_top *b){
  huffman_top t = *a; 
  *a = *b;
  *b = t;
}
//Als je iets goed wilt (of gedaan op een manier dat je wilt snappen) doe het zelf.
static void ssort(huffman_top **rij, int begin, int einde){
  if(einde > begin+1){
    //    int piv = rij[begin]; //piv is eerste element van rij
    huffman_top *piv = rij[begin];
    int links = begin+1;
    int rechts = einde;
    while(links < rechts){
      if(rij[links]->weight >= piv->weight) links++;
      else swap(rij[links],rij[--rechts]);
    }
    swap(rij[--links],rij[begin]);
    ssort(rij,begin,links);
    ssort(rij,rechts,einde);
  }
}

void standaard_huffman(char *input_buffer, int lengte, int actie){
  int *freq_tabel = (int*) calloc(255,4);
  huffman_top **huffman_toppen = (huffman_top**)  calloc(lengte, sizeof(huffman_top*));
  huffman_codewoord **code = (huffman_codewoord**) calloc(255,    sizeof(huffman_codewoord*)); //het maximaal aantal code die we kunnen hebben is 255, een macrosymbool is 1 ascii teken -> 1byte.
  
  int number_of_huffman_top = 0; 
  if(actie){
    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    for(int i = 0; i < lengte; i++){
      freq_tabel[(int)input_buffer[i]]++;
    }
    for(int i = 0; i < 255; i++){
      code[i] = (huffman_codewoord*) calloc(1,sizeof(huffman_codewoord));
      code[i]->number_of_bits = 1;
      code[i]->code = 0;

      if(freq_tabel[i] > 0){
	huffman_toppen[number_of_huffman_top] = (huffman_top*) calloc(1,sizeof(huffman_top));
	huffman_toppen[number_of_huffman_top]->value = (char*) calloc(1,sizeof(char));
	huffman_toppen[number_of_huffman_top]->aantal_elementen = 1;
	memcpy(huffman_toppen[number_of_huffman_top]->value,&i,4);
	huffman_toppen[number_of_huffman_top]->weight = freq_tabel[i];
	number_of_huffman_top++;
      }
    }
    //super sort?
    ssort(huffman_toppen, 0,number_of_huffman_top);
    for(int i = number_of_huffman_top; i > 1; i--){
      printf("index: %d - symbool: %c - weight: %d \n", i, huffman_toppen[i-1]->value[0], huffman_toppen[i-1]->weight);

      for(int k = 0; k  < huffman_toppen[i-1]->aantal_elementen; k++){
	  code[(unsigned int)huffman_toppen[i-1]->value[k]]->code <<= 1;
	  }

    for(int k = 0; k  < huffman_toppen[i-2]->aantal_elementen; k++){
      code[(unsigned int)huffman_toppen[i-2]->value[k]]->code <<= 1;
      code[(unsigned int)huffman_toppen[i-2]->value[k]]->code |= 0x0001;
    }
    
      printf("%d \n", code[(unsigned int)huffman_toppen[i-1]->value[0]]->code);
      printf("%d \n", code[(unsigned int)huffman_toppen[i-2]->value[0]]->code);
      char *t = (char*) malloc(sizeof(char)*huffman_toppen[i-2]->aantal_elementen);
      memcpy(t,huffman_toppen[i-2]->value, huffman_toppen[i-2]->aantal_elementen);
      huffman_toppen[i-2]->value = (char*) realloc(huffman_toppen[i-2]->value, huffman_toppen[i-2]->aantal_elementen+huffman_toppen[i-1]->aantal_elementen);
      

      memcpy(huffman_toppen[i-2]->value, t,huffman_toppen[i-2]->aantal_elementen);
      memcpy(huffman_toppen[i-2]->value + huffman_toppen[i-2]->aantal_elementen, huffman_toppen[i-1]->value,huffman_toppen[i-1]->aantal_elementen);
      huffman_toppen[i-2]->aantal_elementen += huffman_toppen[i-1]->aantal_elementen;
      huffman_toppen[i-2]->weight += huffman_toppen[i-1]->weight;
      
      int k = 1;
      while(huffman_toppen[i-2]->weight > huffman_toppen[(i-2)-k] && (i-2)-k >= 0){
	swap(huffman_toppen[(i-2)],huffman_toppen[(i-2)-k]);
	k++;
      }
      
      return 0;
    }
    
  }else{
    //decodeer
  }

  for(int i = 0; i < number_of_huffman_top; i++){
    free(huffman_toppen[i]->value);
    free(huffman_toppen[i]);
  }
  for(int i = 0; i < 255; i++){
    free(code[i]);
  }
  free(freq_tabel);
  free(huffman_toppen);
  free(code);
}
