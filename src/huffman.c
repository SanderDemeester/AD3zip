#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

void standaard_huffman(unsigned char *input_buffer, uint32_t lengte, uint32_t actie){
  int i; // index variabel
  int k; // interne index variabel
  //  int j; // interne index variabel


  uint32_t number_of_bytes_needed = 0; //het aantal bytes nodig in de output buffer
  uint32_t swap_c = 0; //tijdens het swap proces moeten we de oude postie bijhouden adhv een offset.
  uint32_t *freq_tabel = (uint32_t*) calloc(255,sizeof(uint32_t)); //de freq tabel.
  huffman_top **huffman_toppen = (huffman_top**)  calloc(lengte, sizeof(huffman_top*)); //bijhouden van onze huffman toppen.

  
  /*********************************************************************************************************************************************/
  /* Voor elk niet null element in onze huffman boom nemen we 5 bytes, 1 byte voor het teken zelf, en 4 bytes om het codewoord voor te stellen */
  /* Deze 4 bytes is voor het codewoord die een lengte heeft O(n-1) het slechtste geval							       */
  /*********************************************************************************************************************************************/
  unsigned char * huffman_boom_zend_string = (unsigned char*) calloc((lengte*5),sizeof(unsigned char)); 
  huffman_codewoord **code = (huffman_codewoord**) calloc(255, sizeof(huffman_codewoord*)); //het maximaal aantal code die we kunnen hebben is 255, een macrosymbool is 1 ascii teken -> 1byte.
  uint32_t offset = 0;
  char *output_buffer = NULL;
  uint32_t number_of_huffman_top = 0; // het aantal huffman toppen 
  if(actie){
    //encodeer
    //Opbouwen van een freqentie tabel om de huffman boom te maken.
    
    for(i = 0; i < lengte; i++){
      freq_tabel[(uint32_t)input_buffer[i]]++;
    }
    
    for(i = 0; i < 255; i++){
      code[i] = (huffman_codewoord*) calloc(1,sizeof(huffman_codewoord));
      code[i]->number_of_bits = 1;
      code[i]->code = 0;

      if(freq_tabel[i] > 0){
	huffman_toppen[number_of_huffman_top] = (huffman_top*) calloc(1,sizeof(huffman_top));
	huffman_toppen[number_of_huffman_top]->value = (char*) calloc(1,sizeof(char));
	huffman_toppen[number_of_huffman_top]->aantal_elementen = 1;
	unsigned char l = (unsigned char)i;
	memcpy(huffman_toppen[number_of_huffman_top]->value,&l,1);
	huffman_toppen[number_of_huffman_top]->weight = freq_tabel[i];
	number_of_huffman_top++;

	memcpy(huffman_boom_zend_string + offset,&l,sizeof(unsigned char));

	offset += 1;
	memcpy(huffman_boom_zend_string + offset, &freq_tabel[i],sizeof(uint32_t));
	offset += sizeof(uint32_t);
      }
    }


    number_of_huffman_top--; //start from 0
    //super sort?
    ssort(huffman_toppen, 0,number_of_huffman_top);
#ifdef debug
    printf("aantal toppen in huffman array: %d \n", number_of_huffman_top);
    for(i = number_of_huffman_top; i >= 0; i--){
      printf("index: %d - weight: %d && symbool: %c \n",i, huffman_toppen[i]->weight, huffman_toppen[i]->value[0]);
    }
#endif
    /***************************************************************************************/
    /* Overlopen van al onze toppen in de huffman code opstellen			   */
    /* Onze stop voorwaarde hier is "1" omdat er maar 1 top mag overblijven (onze wortel). */
    /***************************************************************************************/
    for(i = number_of_huffman_top; i >= 1 ; i--){
      
#ifdef debug
     printf("index: %d \n", i);
     printf("huffman code voor element: %d \n", i);
#endif
     for(k = 0; k < huffman_toppen[i]->aantal_elementen; k++){
#ifdef debug
       printf("code voor symbool: %c -> %d \n", huffman_toppen[i]->value[k],code[(int)huffman_toppen[i]->value[k]]->code);
#endif

       code[(uint32_t)huffman_toppen[i]->value[k]]->code <<= 1;
       code[(uint32_t)huffman_toppen[i]->value[k]]->number_of_bits++; 

#ifdef debug
       printf("code voor symbool: %c -> %d \n", huffman_toppen[i]->value[k],code[(uint32_t)huffman_toppen[i]->value[k]]->code);
       printf("%d \n", ( uint32_t)huffman_toppen[i]->value[k]);
#endif
     }
#ifdef debug
     printf("huffman code voor element: %d \n", i-1);
#endif
     for(k = 0; k  < huffman_toppen[i-1]->aantal_elementen; k++){
       
	code[(uint32_t)huffman_toppen[i-1]->value[k]]->code <<= 1; //shift in zero bit
	code[(uint32_t)huffman_toppen[i-1]->value[k]]->code |= (1<<0); //flip first bit.
	code[(uint32_t)huffman_toppen[i-1]->value[k]]->number_of_bits++;
#ifdef debug
	printf("code voor symbool: %c -> %d \n", huffman_toppen[i-1]->value[k],code[(uint32_t)huffman_toppen[i-1]->value[k]]->code);
#endif
     }
     

#ifdef debug
     printf("%d \n", code[( uint32_t)huffman_toppen[i]->value[0]]->code);
     printf("%d \n", code[( uint32_t)huffman_toppen[i]->value[0]]->code);
#endif
     
     unsigned char *t = (unsigned char*) malloc(sizeof(unsigned char)*huffman_toppen[i-1]->aantal_elementen);
     memcpy(t,huffman_toppen[i-1]->value, huffman_toppen[i-1]->aantal_elementen);
     huffman_toppen[i-1]->value = (unsigned char*) realloc(huffman_toppen[i-1]->value, huffman_toppen[i-1]->aantal_elementen+huffman_toppen[i]->aantal_elementen);

          
     memcpy(huffman_toppen[i-1]->value, t,huffman_toppen[i-1]->aantal_elementen);
     memcpy(huffman_toppen[i-1]->value + huffman_toppen[i-1]->aantal_elementen, huffman_toppen[i]->value,huffman_toppen[i]->aantal_elementen);
     huffman_toppen[i-1]->aantal_elementen += huffman_toppen[i]->aantal_elementen;
     huffman_toppen[i-1]->weight += huffman_toppen[i]->weight;
     free(t);
     
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

      k = 1; //offset naar andere top in array
      swap_c = 0; //index offset
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
#ifdef debug
	printf("opvullen van lege plaats\n");
#endif
	swap(huffman_toppen[i+1],huffman_toppen[i]);
      }
    }

#ifdef debug
    printf("stop huffman coding\n");
#endif
    for(i = 0; i < lengte; i++) number_of_bytes_needed += code[(uint32_t)input_buffer[i]]->number_of_bits;
    number_of_bytes_needed = (uint32_t)ceil(number_of_bytes_needed/8);
    if(number_of_bytes_needed == 0) number_of_bytes_needed++;
    
    output_buffer = (char*) calloc(number_of_bytes_needed,sizeof(char));
    //    printf("aantal bytes nodig: %d \n", number_of_bytes_needed);
    
    uint32_t index = 0; //De huidige index voor de output array
    uint32_t aantal_bits_in_huidige_byte = 0; //aantal bits die op dit moment in de array zitten
    uint32_t plaats_over = 8; //hoeveel vrije plaatsen er over zijn
    uint32_t rest = 0; //als we over een grens gaan moeten we weten hoeveel we moeten plaatsen in het volgende stuk
    unsigned char mask = 0xFF; //de mask waarmee we bit grenzen afschermen.
    
    unsigned int  codewoord = '\0'; //om simpel te werken hebben we extra velden 
    uint32_t b = 0;
    huffman_header *header = (huffman_header*) malloc(sizeof(huffman_header));
    
    for(i = 0; i < lengte; i++){
      //Om simpel te werken slaan we ons codewoord en aantal bits op 
      codewoord = code[(unsigned int) input_buffer[i]]->code;
      b = code[(unsigned int) input_buffer[i]]->number_of_bits;
      
      //Als het aantal bits in de huidige output byte nog plaats heeft voor een deel van ons volgende codewoord
      //Gaan we de eerste 8 - aantal_bits_in_huidige_byte bits van ons codewoord in de huidige byte opslaan
      if(aantal_bits_in_huidige_byte + b > 8){
	//Bepaal hoeveel plaats er nog vrij is in onze huidige output byte
	plaats_over = 8 - aantal_bits_in_huidige_byte; 
	
	//Mask het aantal bits af dat nog vrij is, dit is 2^{plaats nog vrij}-1. BV: 3bits, 2^3-1-> 111
	mask = (1 << plaats_over) - 1;
	
	//Reserveer genoeg plaats in output byte 
	output_buffer[index] <<= plaats_over;
	
	//OR het deel van ons codewoord in de output byte geshift over het verschil in lengte van 
	//1. lengte codewoord
	//2. aantal plaatsen
	// ge-and met ons masker
	output_buffer[index] |= ((codewoord >> b) - plaats_over) & mask;

	//Ga over naar nieuwe output byte
	index++;
	
	//Het vrije bits in onze nieuwe output byte is terug maximaal
	aantal_bits_in_huidige_byte = 0;
	
	//Hoeveel van het huidige codewoord hebben we nog niet in onze output array opgenomen?
	rest = b - plaats_over;
	
	//Bepaal terug een masker om de rest van ons codewoord eruit te halen.
	mask = (1 << rest) - 1;

	//Maak genoeg plaats vrij in onze nieuwe output byte
	output_buffer[index] <<= rest;

	//OR de rest van het codewoord, afgeschermd door de byte in onze nieuwe output byte
	output_buffer[index] |= codewoord & mask;
	
	//Zet het aantal bits dat wordt gebruikt in de output byte op de rest
	aantal_bits_in_huidige_byte = rest;
	plaats_over = 8 - rest;
	rest = 0;
      }else{
	output_buffer[index] <<= b;
	output_buffer[index] |= codewoord;
	aantal_bits_in_huidige_byte += b;
	plaats_over -= b;
	if(aantal_bits_in_huidige_byte == 8){
	  index++;
	  aantal_bits_in_huidige_byte = 0;
	  plaats_over = 8;
	}
      }
    }

    header->huffman_block_len = number_of_bytes_needed + (lengte*5); //de totale lengte van dit huffman block
    header->huffman_code_len = number_of_bytes_needed; //de lengte van de huffman code
    header->huffman_boom_len = lengte*5; //de lengte van de huffman boom

    
    fwrite(header,1,12,stdout); //print header
    fwrite(huffman_boom_zend_string,1,lengte*5,stdout); //print huffman boom
    fwrite(output_buffer, 1,number_of_bytes_needed,stdout); //print huffman code

    free(header);
    free(output_buffer);
    free(huffman_boom_zend_string);
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

