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
  int i;
  int k;
  int j;
  int swap_c = 0;
  int *freq_tabel = (int*) calloc(255,4);
  huffman_top **huffman_toppen = (huffman_top**)  calloc(lengte, sizeof(huffman_top*));
  huffman_codewoord **code = (huffman_codewoord**) calloc(255,    sizeof(huffman_codewoord*)); //het maximaal aantal code die we kunnen hebben is 255, een macrosymbool is 1 ascii teken -> 1byte.
  
  int number_of_huffman_top = 0; 
  if(actie){

    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    
    for(i = 0; i < lengte; i++){
      freq_tabel[(int)input_buffer[i]]++;
    }
    
    for(i = 0; i < 255; i++){
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
    number_of_huffman_top--; //start from 0
    //super sort?
    ssort(huffman_toppen, 0,number_of_huffman_top);



    printf("aantal toppen in huffman array: %d \n", number_of_huffman_top);
    for(i = number_of_huffman_top; i >= 0; i--){
      printf("index: %d - weight: %d && symbool: %c \n",i, huffman_toppen[i]->weight, huffman_toppen[i]->value[0]);
    }



    
    for(i = number_of_huffman_top; i >= 1 ; i--){
      
#ifdef debug
      printf("index: %d \n", i);
#endif

      printf("huffman code voor element: %d \n", i);
      for(k = 0; k < huffman_toppen[i]->aantal_elementen; k++){
	printf("code voor symbool: %c -> %d \n", huffman_toppen[i]->value[k],code[(int)huffman_toppen[i]->value[k]]->code);
	  code[(int)huffman_toppen[i]->value[k]]->code <<= 1;
	  printf("code voor symbool: %c -> %d \n", huffman_toppen[i]->value[k],code[(int)huffman_toppen[i]->value[k]]->code);
#ifdef debug
	  printf("%d \n", ( int)huffman_toppen[i]->value[k]);
#endif 
      }
      printf("huffman code voor element: %d \n", i-1);
      for(k = 0; k  < huffman_toppen[i-1]->aantal_elementen; k++){
	code[( int)huffman_toppen[i-1]->value[k]]->code <<= 1;
	code[( int)huffman_toppen[i-1]->value[k]]->code |= 0x0001;
	printf("code voor symbool: %c -> %d \n", huffman_toppen[i-1]->value[k],code[(int)huffman_toppen[i-1]->value[k]]->code);
      }

      
#ifdef debug
      printf("%d \n", code[( int)huffman_toppen[i]->value[0]]->code);
      printf("%d \n", code[( int)huffman_toppen[i]->value[0]]->code);
#endif
      
      char *t = (char*) malloc(sizeof(char)*huffman_toppen[i-1]->aantal_elementen);
      memcpy(t,huffman_toppen[i-1]->value, huffman_toppen[i-1]->aantal_elementen);
      huffman_toppen[i-1]->value = (char*) realloc(huffman_toppen[i-1]->value, huffman_toppen[i-1]->aantal_elementen+huffman_toppen[i]->aantal_elementen);
      
      memcpy(huffman_toppen[i-1]->value, t,huffman_toppen[i-1]->aantal_elementen);
      memcpy(huffman_toppen[i-1]->value + huffman_toppen[i-1]->aantal_elementen, huffman_toppen[i]->value,huffman_toppen[i]->aantal_elementen);
      huffman_toppen[i-1]->aantal_elementen += huffman_toppen[i]->aantal_elementen;
      huffman_toppen[i-1]->weight += huffman_toppen[i]->weight;
      
#ifdef debug
      printf("de top i-1 achter de merge\n");
      printf("gewicht: %d \n", huffman_toppen[i-1]->weight);
      printf("aantal elementen: %d \n", huffman_toppen[i-1]->aantal_elementen);
      printf("begin to print aantal elementen\n");
      for(k = 0; k < huffman_toppen[i-1]->aantal_elementen; k++){
	printf("%d, %c \n", k, huffman_toppen[i-1]->value[k]);
      }
      printf("einde\n");
      printf("print elementen\n");
      
      for(j = number_of_huffman_top; j >= 0; j--){
	if(huffman_toppen[j] == NULL)
	  printf("elementen: %d is NULL\n",j);
	else
	  printf("index: %d && weight: %d - first symbool: %c \n",j,huffman_toppen[j]->weight, huffman_toppen[j]->value[0]);
      }
      
      printf("einde printen elementen\n");
#endif

      k = 1;
      swap_c = 0;
      while((i-1)-k >= 0 && huffman_toppen[(i-1)-swap_c]->weight > huffman_toppen[(i-1)-k]->weight){

#ifdef debug
	printf("voor swap, index: %d\n",i);
	for(j = number_of_huffman_top; j >= 0; j--){
	  if(huffman_toppen[j] == NULL)
	    printf("element: %d is NULL\n",j);
	  else
	    printf("index: %d && weight: %d - first symbool: %c \n",j,huffman_toppen[j]->weight, huffman_toppen[j]->value[0]);
	}
	printf("%d && %d\n",(i-1)-swap_c, (i-1)-k);
#endif
	swap(huffman_toppen[(i-1)-swap_c],huffman_toppen[(i-1)-k]);
	swap_c++;
#ifdef debug
	printf("na swap\n");
	for(j = number_of_huffman_top; j >= 0; j--){
	  if(huffman_toppen[j] == NULL)
	    printf("element %d is NULL\n",j);
	  else
	    printf("index: %d && weight: %d - first symbool: %c \n",j,huffman_toppen[j]->weight, huffman_toppen[j]->value[0]);
	}
#endif
	k++;
	if((i-1)-k < 0)	  break;
#ifdef debug
	printf("%d && %d\n",(i-1)-swap_c, (i-1)-k);
	printf("%d > %d\n", huffman_toppen[(i-1)-swap_c]->weight, huffman_toppen[(i-1)-k]->weight);
#endif


      }

      free(huffman_toppen[i]->value);
      free(huffman_toppen[i]);


      huffman_toppen[i] = NULL; //het element waarmee we zijn samengevoegd mag weg
      if((i+1 >= number_of_huffman_top && i+1 <= number_of_huffman_top) && huffman_toppen[i+1] != NULL){
	printf("opvullen van lege plaats\n");
	swap(huffman_toppen[i+1],huffman_toppen[i]);
      }
    }

#ifdef debug
    printf("stop huffman coding\n");
#endif

    for(i = 0; i < lengte; i++){
      printf("plaintext word: %c -> codeword: %d\n", input_buffer[i], code[input_buffer[i]]->code);
      
    }

  }else{
    //decodeer
  }

  free(huffman_toppen[0]->value);
  free(huffman_toppen[0]);


  for(i = 0; i < 255; i++){
    free(code[i]);
  }
  free(freq_tabel);
  free(huffman_toppen);
  free(code);
}
