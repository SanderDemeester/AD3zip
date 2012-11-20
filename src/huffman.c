#include "header/huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**********************************************************************************************************************************************/
/* We weten dat de kans groter is dat de symbolen met een hogeren freqentie verder zullen staan in de input_buffer->symbolen later in de rij  */
/* zullen hogere freqentie hebben													      */
/**********************************************************************************************************************************************/

static void p_b(unsigned char v){
  for(int i = 7;  i >= 0 ;i--){
    printf("%d", (v >> i) & 0x01);
  }
  printf("\n");

}
static void swap(huffman_top *a, huffman_top *b){
  huffman_top t = *a; 
  *a = *b;
  *b = t;
}
unsigned int reverse(register unsigned int x)
{
  x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
  x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
  x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
  x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
  return((x >> 16) | (x << 16));

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

  if(actie){
    //encodeer
  int i; // index variabel
  uint32_t number_of_bytes_needed = 0; //het aantal bytes nodig in de output buffer

  huffman_top **huffman_toppen = (huffman_top**)  calloc(lengte, sizeof(huffman_top*)); //bijhouden van onze huffman toppen.
  uint32_t *freq_tabel = (uint32_t*) calloc(255,sizeof(uint32_t)); //de freq tabel.
  unsigned char * huffman_boom_zend_string = (unsigned char*) calloc((lengte*5),sizeof(unsigned char)); 

  
  /*********************************************************************************************************************************************/
  /* Voor elk niet null element in onze huffman boom nemen we 5 bytes, 1 byte voor het teken zelf, en 4 bytes om het codewoord voor te stellen */
  /* Deze 4 bytes is voor het codewoord die een lengte heeft O(n-1) het slechtste geval							       */
  /*********************************************************************************************************************************************/

  huffman_codewoord **code = (huffman_codewoord**) calloc(255, sizeof(huffman_codewoord*)); //het maximaal aantal code die we kunnen hebben is 255, een macrosymbool is 1 ascii teken -> 1byte.
  uint32_t huffman_zend_string_offset = 0;
  unsigned char *output_buffer = NULL;
  uint32_t number_of_huffman_top = 0; // het aantal huffman toppen 
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
	huffman_toppen[number_of_huffman_top]->value = (unsigned char*) calloc(1,sizeof(unsigned char));
	huffman_toppen[number_of_huffman_top]->aantal_elementen = 1;
	unsigned char l = (unsigned char)i;
	memcpy(huffman_toppen[number_of_huffman_top]->value,&l,1);
	huffman_toppen[number_of_huffman_top]->weight = freq_tabel[i];
	number_of_huffman_top++;
	
	memcpy(huffman_boom_zend_string + huffman_zend_string_offset,&l,sizeof(unsigned char));

	huffman_zend_string_offset += 1;
	memcpy(huffman_boom_zend_string + huffman_zend_string_offset, &freq_tabel[i],sizeof(uint32_t));
	huffman_zend_string_offset += sizeof(uint32_t);

      }
    }

    number_of_huffman_top--; //start from 0
    //super sort?
    ssort(huffman_toppen, 0,number_of_huffman_top);

    build_huffmancode(huffman_toppen,code,number_of_huffman_top);
    
    for(i = 0; i < lengte; i++) number_of_bytes_needed += code[(uint32_t)input_buffer[i]]->number_of_bits;
    number_of_bytes_needed = (uint32_t)ceil(number_of_bytes_needed/8);
    if(number_of_bytes_needed == 0) number_of_bytes_needed++;
    
    output_buffer = (unsigned char*) calloc(number_of_bytes_needed+1,sizeof(unsigned char));
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
      codewoord = reverse(codewoord);
      b = code[(unsigned int) input_buffer[i]]->number_of_bits;
      codewoord = (codewoord >> (32-b));

      /* printf("%d\n",b); */
      /* for(int k = b-1; k >= 0; k--){ */
      /* 	printf("%d", (codewoord >> k) & 0x01); */
      /* } */

      /* printf("\n"); */
      
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
	//	p_b(output_buffer[index]);
	index++;
	
	//Het vrije bits in onze nieuwe output byte is terug maximaal
	aantal_bits_in_huidige_byte = 0;
	
	//Hoeveel van het huidige codewoord hebben we nog niet in onze output array opgenomen?
	rest = b - plaats_over;
	
	//Bepaal terug een masker om de rest van ons codewoord eruit te halen.
	mask = (1 << rest) - 1;

	output_buffer[index] <<= rest;

	//OR de rest van het codewoord, afgeschermd door de byte in onze nieuwe output byte
	output_buffer[index] |= codewoord & mask;
	
	//Zet het aantal bits dat wordt gebruikt in de output byte op de rest
	aantal_bits_in_huidige_byte = rest;
	plaats_over = 8 - rest;
	rest = 0;
	//	p_b(output_buffer[index]);
      }else{
	output_buffer[index] <<= b;
	output_buffer[index] |= codewoord;
	aantal_bits_in_huidige_byte += b;
	plaats_over -= b;
	//	p_b(output_buffer[index]);
	if(aantal_bits_in_huidige_byte == 8){
	  index++;
	  aantal_bits_in_huidige_byte = 0;
	  plaats_over = 8;
	  //p_b(output_buffer[index]);
	}
      }
    }

    /* for(int i = 0; i <= number_of_bytes_needed; i++){ */
    /*   p_b(output_buffer[i]); */
    /* } */
    /* printf("%d \n", number_of_bytes_needed); */

    header->huffman_block_len = (number_of_bytes_needed+1) + huffman_zend_string_offset; //de totale lengte van dit huffman block
    header->huffman_code_len = number_of_bytes_needed; //de lengte van de huffman code
    header->huffman_boom_len = huffman_zend_string_offset; //de lengte van de huffman boom

    /* printf("%d \n", header->huffman_block_len); */
    /* printf("%d \n", header->huffman_code_len); */
    /* printf("%d \n", header->huffman_boom_len); */

    
    fwrite(header,1,12,stdout); //print header
    fwrite(huffman_boom_zend_string,1,huffman_zend_string_offset,stdout); //print huffman boom
    fwrite(output_buffer, 1,number_of_bytes_needed+1,stdout); //print huffman code

    free(header);
    free(output_buffer);
    free(huffman_boom_zend_string);
    
    free(huffman_toppen[0]->value);
    free(huffman_toppen[0]);
  
  
  for(i = 0; i < 255; i++){
    free(code[i]);
  } 
  free(freq_tabel);
  free(huffman_toppen);
  free(code);
  
  }else{
    uint32_t huffman_block_len = 0;
    uint32_t huffman_code_len = 0;
    uint32_t huffman_boom_len = 0;
    unsigned char *codewoorden = NULL;
    huffman_top **huffman_toppen = NULL;
    unsigned char * huffman_boom_zend_string = NULL;
    huffman_codewoord **code = NULL;
    huffman_tree_element** list_to_free = (huffman_tree_element**) calloc(255,sizeof(huffman_tree_element*));
    int aantal_element_to_free = 0;
    

    int index = 0;
    uint32_t waarde = 0;
    
    memcpy(&huffman_block_len, input_buffer, 4);
    input_buffer += 4;
    memcpy(&huffman_code_len, input_buffer,4);
    input_buffer += 4;
    memcpy(&huffman_boom_len, input_buffer, 4);
    input_buffer += 4;

    
    huffman_toppen = (huffman_top**) calloc((huffman_boom_len/5),sizeof(huffman_top*));
    huffman_boom_zend_string = (unsigned char*) calloc(huffman_boom_len,sizeof(unsigned char));
    
    //Dit moet 255 zijn. 
    code = (huffman_codewoord**) calloc(255,sizeof(huffman_codewoord*));

    memcpy(huffman_boom_zend_string, input_buffer, huffman_boom_len); //kopieer de huffman boom die we hebbeb gekregen van de input buffer naar de zend string.
    input_buffer += huffman_boom_len; 
   
    codewoorden = (unsigned char*) malloc(sizeof(unsigned char)*huffman_code_len+1);
    memcpy(codewoorden,input_buffer, huffman_code_len+1);
    for(int i = 0; i <= huffman_code_len; i++){
      p_b(codewoorden[i]);
    }

    input_buffer += huffman_code_len; //input buffer is at his end point.

    //De huidige positie is nu de start van alle codewoorden en de huffman boom, dit heeft een lengte: huffman_block_len
    printf("huffman_block_len: %d \n", huffman_block_len);
    printf("huffman_code_len : %d \n", huffman_code_len);
    printf("huffman_boom_len : %d \n", huffman_boom_len);
    
    for(int i = 0; i < 255; i++){
      code[i] = (huffman_codewoord*) calloc(1,sizeof(huffman_codewoord));
      code[i]->number_of_bits = 1;
      code[i]->code = 0;
    }

    for(int i = 0; i < huffman_boom_len/5; i++){
      
      memcpy(&index, huffman_boom_zend_string,1);
      memcpy(&waarde,huffman_boom_zend_string+1,4);
      
      huffman_toppen[i] = (huffman_top*) calloc(1,sizeof(huffman_top));
      huffman_toppen[i]->value = (unsigned char*) calloc(1,sizeof(unsigned char));
      huffman_toppen[i]->aantal_elementen = 1;

      unsigned char l = (unsigned char)index;

      memcpy(huffman_toppen[i]->value,&l,1);
      huffman_toppen[i]->weight = waarde;      

      huffman_boom_zend_string+=5;
    }
    //het sorteren van de huffman toppen
    ssort(huffman_toppen,0,(huffman_boom_len/5)-1);
    
    build_huffmancode(huffman_toppen,code,(huffman_boom_len/5)-1);
    
    //Maak een echte tree voor het decoderen
    
    huffman_tree_element *root = (huffman_tree_element*) calloc(1,sizeof(huffman_tree_element));
    root->is_root = 1;
    root->bit = 0; //default for root
    root->is_blad = 0; //geen blad
    root->value = NULL;
    root->links = NULL;
    root->rechts = NULL;
    list_to_free[aantal_element_to_free++] = root;

    /* for(int i = 0; i < 255; i++){ */
    /*   if(code[i]->number_of_bits > 1){ */
    /* 	code[i]->code = reverse(code[i]->code); */
    /* 	code[i]->code = (code[i]->code >> (32-code[i]->number_of_bits)); */
    /* 	for(int k = code[i]->number_of_bits-1; k >= 0; k--){ */
    /* 	  printf("%d ", (code[i]->code >> k) & 0x01); */
    /* 	} */
    /* 	printf("\n"); */
    /*   } */
    /* } */
    
    //1->rechts
    //0->links
    for(int i = 0; i < 255; i++){
      if(code[i]->number_of_bits > 1){
	code[i]->code = reverse(code[i]->code);
	code[i]->code = (code[i]->code >> (32-code[i]->number_of_bits));
	huffman_tree_element *w = root;

	int b = 0;
	/* printf("begin seq for value: %d\n",code[i]->code); */
	/* printf("aantal bits: %d \n", code[i]->number_of_bits); */
	for(int k = code[i]->number_of_bits-1; k >= 0; k--){
	  b = (code[i]->code >> k) & 0x01;
	  /* printf("bit: %d \n", b); */
	  if(b){
	    //we moeten naar rechts
	    if(w->rechts == NULL){
	      //rechts is null
	      w->rechts = (struct huffman_tree_element*) calloc(1,sizeof(huffman_tree_element));

	      list_to_free[aantal_element_to_free++] = w->rechts;
	      w = (huffman_tree_element*)w->rechts;
	      w->bit = b;
	      w->value = NULL;
	      w->is_root = 0;
	      w->links = NULL;
	      w->rechts = NULL;
	    }else{
	      //rechts is niet null
	      w = (huffman_tree_element*)w->rechts;
	    }
	  }else{
	    //we moeten naar links
	    if(w->links == NULL){
	      //w->links is NULL
	      w->links = (struct huffman_tree_element*) calloc(1,sizeof(huffman_tree_element));

	      list_to_free[aantal_element_to_free++] = w->links;
	      w = (huffman_tree_element*)w->links;
	      w->bit = b;
	      w->is_blad = 0;
	      w->is_root = 0;
	      w->links = NULL;
	      w->rechts = NULL;
	      w->value = NULL;
	    }else{
	      //w->links is niet NULL
	      w = (huffman_tree_element*)w->links;	      
	    }
	  }
	}
	//	printf("end seq\n");
	w->is_blad = 1;
	w->value = (char*) malloc(sizeof(char));
	memcpy(w->value,&i,1);
      }
    }

    unsigned char c = codewoorden[0];
    huffman_tree_element *w = root;
    int b;
    int b_pos = 7;
    int i = 0;
    while(i < huffman_code_len){
      if(b_pos <= -1){
	i++;
	c = codewoorden[i];
	b_pos = 7;
      }
      
      printf("begin woord: %d \n",c);

      while(!w->is_blad){
	b = ((c >> b_pos) & 0x01);
	printf("bit: %d \n", b);
	if(b){
	  //ga rechts
	  if(w->rechts == NULL){
	    printf("einde van de code\n");
	    w = NULL; //signal
	    break;
	  }
	  w = (huffman_tree_element*)w->rechts;
	}else{
	  if(w->links == NULL){
	    printf("einde van de code\n");
	    w = NULL; //signal
	    break;
	  }
	  w = (huffman_tree_element*)w->links;
	}
	b_pos--;
	if(b_pos == -1){
	  printf("einde byte\n");
	  break;
	}
      }
      printf("einde woord\n");
      printf("aantal bits over: %d \n", b_pos); 
      if(w == NULL) break;
      if(w->is_blad){
	printf("%c \n", w->value);
	printf("reset root\n");      
	w = root;
      }
    }
    

    for(int i = 0; i < 255; i++) free(code[i]);
    
    free(huffman_toppen[0]->value);
    free(huffman_toppen[0]);
    
    free(huffman_toppen);
    free(huffman_boom_zend_string-huffman_boom_len);  
    free(code);
    
    free(codewoorden);

    //FREE huffxmantree!!!
    for(int i = 0; i < aantal_element_to_free; i ++){
      printf("%d \n", i);
      if(list_to_free[i]->is_blad) free(list_to_free[i]->value);
      
      free(list_to_free[i]);
    }
    free(list_to_free);
  }
}

void build_huffmancode(huffman_top **huffman_toppen,
		       huffman_codewoord **code,
		       int number_of_huffman_top){
      /***************************************************************************************/
    /* Overlopen van al onze toppen in de huffman code opstellen			   */
    /* Onze stop voorwaarde hier is "1" omdat er maar 1 top mag overblijven (onze wortel). */
    /***************************************************************************************/
  int i;
  int k;
  int swap_c;

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
}
