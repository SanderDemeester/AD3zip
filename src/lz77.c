#include "header/lz77.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef debug
static void test_longest_match(){
  char *t = "ababaababa";
  char *z = "c";
  
  int t_l = 10;
  int z_l = 1;
  
  match_pair *p1 = (match_pair*) calloc(1, sizeof(match_pair));
  match_pair *p2 = (match_pair*) calloc(1, sizeof(match_pair));

  find_longest_match(t,t_l, z,z_l,p1,p2);

  printf("start: %d && len: %d \n", p1->p, p1->l);
  free(p1);
  free(p2);
}
#endif

unsigned char* lz77_encodeer(unsigned char *input_buffer, int len){
  int start_sliding_window = 0;
  int end_sliding_window = 0;

  uint32_t index_huidig_element = 0;
  uint32_t code_woorde_index = 1;

  lz77_codewoord **codewoorden = (lz77_codewoord**) calloc(1, sizeof(lz77_codewoord*));
  lz77_codewoord **realloc_codewoorden = NULL;

  match_pair *p1 = (match_pair*) calloc(1,sizeof(match_pair));
  match_pair *p2 = (match_pair*) calloc(1,sizeof(match_pair));

  unsigned char *output_buffer;

  p1->p = 0;
  p1->l = 0;

  p2->p = 0;
  p2->l = 0;

#ifdef lz77_debug
  printf("hier %c \n", input_buffer[0]);
#endif
  while(index_huidig_element <= len-1){
#ifdef lz77_debug  
    printf("index huidig element: %d  && len: %d \n", index_huidig_element, len);
    printf("print sliding window\n");
    for(int i = 0; i < end_sliding_window - start_sliding_window; i++){
      printf("%c \n", input_buffer[start_sliding_window+i]);
    }
    
    printf("volgende vrije byte: %c \n", input_buffer[end_sliding_window]);
    printf("de lengte van de text bufer: %d \n", len-(end_sliding_window));
#endif
    find_longest_match(input_buffer+(end_sliding_window), //het eerste volgende byte volgend op het einde van het sliding window
		       len-(end_sliding_window), //de volledige lengte van de buffer - de lengte van het sliding window
		       input_buffer + start_sliding_window, //start van sliding window
		       end_sliding_window - start_sliding_window,
		       p1,
		       p2);		       
    end_sliding_window++;

    if(p1->l > 0){
#ifdef lz77_debug
      printf("(%d,%d,%c)\n", p1->p, p1->l,input_buffer[index_huidig_element+p1->l]);
#endif

      codewoorden[code_woorde_index-1] = (lz77_codewoord*) malloc(sizeof(lz77_codewoord));
      codewoorden[code_woorde_index-1]->p = p1->p; //pos van match in sliding window
      codewoorden[code_woorde_index-1]->l = p1->l; //lengte van de gevonden deelstring
      if(index_huidig_element+p1->l >= len-1)
	codewoorden[code_woorde_index-1]->x = input_buffer[index_huidig_element+p1->l-1];
      else
	codewoorden[code_woorde_index-1]->x = input_buffer[index_huidig_element+p1->l];

      code_woorde_index++;
      realloc_codewoorden = (lz77_codewoord**) realloc(codewoorden,code_woorde_index*(sizeof(lz77_codewoord*)));
      if(realloc_codewoorden != NULL)
	codewoorden = realloc_codewoorden;
      
      index_huidig_element += p1->l+1;
      end_sliding_window += p1->l; //update sliding window met de lengte van de match.
    }else{
#ifdef lz77
      printf("(%d,%d,%c)\n", 0,0,input_buffer[index_huidig_element]);
#endif
      codewoorden[code_woorde_index-1] = (lz77_codewoord*) malloc(sizeof(lz77_codewoord));
      codewoorden[code_woorde_index-1]->p = 0;
      codewoorden[code_woorde_index-1]->l = 0;
      codewoorden[code_woorde_index-1]->x = input_buffer[index_huidig_element];

      code_woorde_index++;
      realloc_codewoorden = (lz77_codewoord**) realloc(codewoorden, code_woorde_index*(sizeof(lz77_codewoord*)));
      if(realloc_codewoorden != NULL)
	codewoorden = realloc_codewoorden;
      index_huidig_element++;
    }
    

    if((end_sliding_window - start_sliding_window) > G)     
      start_sliding_window = end_sliding_window - G;
  }
  
  //fix off by one.
  code_woorde_index--;
  
  //De +4 is om de lengte aan te duiden.
  output_buffer = (unsigned char*) malloc(4+(code_woorde_index*9));
  
  //De eerste 4 bytes van de output_buffer is het aantal lz77 codewoorden er zijn opgeslagen.
  //elke lz77 codewoord is 5bytes lang.
  memcpy(output_buffer, &code_woorde_index, 4);
  output_buffer+=4;
  
  //FREEDOM!! oooh FREEDOM!!!
  for(int i = 0; i < code_woorde_index; i++){
    memcpy(output_buffer,&codewoorden[i]->p,sizeof(uint32_t)); //eerst de start pos
    output_buffer+=sizeof(uint32_t);
    memcpy(output_buffer,&codewoorden[i]->l,sizeof(uint32_t)); //daarna de lengte
    output_buffer+=sizeof(uint32_t);
    memcpy(output_buffer,&codewoorden[i]->x,sizeof(unsigned char));//daarna de byte zelf
    output_buffer+=sizeof(unsigned char);
    
    free(codewoorden[i]);
  }
  output_buffer -= 4+(code_woorde_index*LZ77_LEN_CODEWOORD);
  free(codewoorden);
  free(p1);
  free(p2);
  return output_buffer;
}

lz77_resultaat* lz77_decoderen(unsigned char*input_buffer, int len){
  //hierin geven we onze bwt vector terug die we hebben geencodeerdt met lz77

  lz77_resultaat*lz77_result = (lz77_resultaat*) malloc(sizeof(lz77_resultaat));
  lz77_result->res = (unsigned char*) calloc(1,sizeof(unsigned char));
  lz77_result->aantal_bytes = 0;
  lz77_codewoord**codewoorden = (lz77_codewoord**) calloc(len/9,sizeof(lz77_codewoord*));
  
  unsigned char*b = NULL;

  int start_sliding_window = 0;
  int end_sliding_window = 0;
  
  for(int i = 0; i < (len/9); i++){
    codewoorden[i] = (lz77_codewoord*) calloc(1,sizeof(lz77_codewoord));
    //   printf("(%d,%d,%c)\n",*input_buffer,*(input_buffer+4),*(input_buffer+8));
    codewoorden[i]->p = *input_buffer; //start pos in sliding window
    codewoorden[i]->l = *(input_buffer+4); //lengte van de match
    codewoorden[i]->x = *(input_buffer+8); //volgende byte
    input_buffer+=LZ77_LEN_CODEWOORD;
  }

  lz77_result->res[0] = codewoorden[0]->x;
  lz77_result->aantal_bytes += codewoorden[0]->l+1;
  b = (unsigned char*) realloc(lz77_result->res,lz77_result->aantal_bytes*sizeof(unsigned char));
  if(b != NULL) lz77_result->res = b;
  else printf("Error\n");

  end_sliding_window++; //sliding window wordt verhoogt.

  //start op 1 en (len/9)-1 omdat we het eerste element al buiten de lus hebben gedaan.
  for(int i = 1;  i < (len/9); i++){

    /* printf("(%d,%d,%c)\n", codewoorden[i]->p,codewoorden[i]->l,codewoorden[i]->x); */
    /* printf("start sliding window:%d && end_sliding_window:%d\n", start_sliding_window,end_sliding_window); */
    /* printf("print sliding window\n"); */
    /* for(int i = 0; i < end_sliding_window - start_sliding_window; i++){ */
    /*   printf("%c \n", codewoorden[i]->x); */
    /* } */
    
    /* printf("lengte volgende codewoord:%d \n", codewoorden[i]->l); */

    //de len is +1 omdat we altijd het extra teken die erop volgt mee moeten opnemen in de string.
    b = (unsigned char*) realloc(lz77_result->res,lz77_result->aantal_bytes+codewoorden[i]->l+1);
    if(b != NULL) lz77_result->res = b;
    
    if(codewoorden[i]->l > 0){
      lz77_result->aantal_bytes+=codewoorden[i]->l+1;
      for(int k = 0; k < codewoorden[i]->l; k++)
	memcpy(lz77_result->res+(end_sliding_window)+k,
	       lz77_result->res+(start_sliding_window+codewoorden[i]->p)+k,
	       1);

      end_sliding_window+=codewoorden[i]->l;
    }else{
      lz77_result->aantal_bytes++;
    }

    memcpy(lz77_result->res+(end_sliding_window),&codewoorden[i]->x,1);
    end_sliding_window++;

    //    lz77_result->aantal_bytes+=codewoorden[i]->l+1;

    if((end_sliding_window - start_sliding_window) > G)     
      start_sliding_window = end_sliding_window - G;    
    /* printf("einde iteratie\n"); */
  }
  
  //  lz77_result->aantal_bytes++;
  
  for(int i = 0; i < len/9; i++)
    free(codewoorden[i]);
  
    
  free(codewoorden);
  input_buffer-= len;
  return lz77_result;
}

//return pos eerste mis_match.
static int vergelijk_strings(unsigned char *z, unsigned char*t, int z_l, int t_l, int start, int gelijke_tekens){
  for(int i = 0; i < z_l; i++){
#ifdef lz77_debug
    printf("vergelijk z[start+i]: %c met t[i]: %c\n", z[start+i], t[i]);
#endif
    //aanpassing aan het originele zoek algoritme. We verschuiven in onze zoekstring, niet in onze tekst
    if(z[start+i] != t[i]){
#ifdef lz77_debug
      printf("ze zijn verschillende, return\n");
#endif
      return i;
    }
#ifdef lz77_debug
    printf("ze zijn gelijk\n");
#endif
  }

  return z_l;
}

static void bereken_V(uint32_t *V, unsigned char *z, uint32_t z_l){
  //z_l = 3-> {0,1,2}
  int start = 1;
  int gelijke_tekens = 0;
  int pos_eerste_mismach = 0;
  V[0] = 1;
  V[1] = 1;
  if(z_l <= 2) return;
  while(start < (z_l-1)){
    pos_eerste_mismach = vergelijk_strings(z,z,z_l,z_l,start,gelijke_tekens);
    if(pos_eerste_mismach == 0) V[start+1] = start+1;
    else
      for(int i = gelijke_tekens+1; i <= pos_eerste_mismach; i++) 
	V[start+i] = start;
    start += V[pos_eerste_mismach];
    if(pos_eerste_mismach == 0) gelijke_tekens = 0; 
    else gelijke_tekens = pos_eerste_mismach-V[pos_eerste_mismach];
  }
  
}

/*We gebruiken ons sliding window als zoekstring, en onze bytes na het sliding window
als onze zoek tekst */
void find_longest_match(unsigned char *t, 
			int t_l, 
			unsigned char *z, 
			int z_l,
			match_pair *p1,
			match_pair *p2){

  uint32_t *V = NULL;
  if(z_l < 2) 
    V = (uint32_t*) calloc(2+1, sizeof(uint32_t));
  else
    V = (uint32_t*) calloc(z_l+1, sizeof(uint32_t));

  if(V == NULL)
    printf("probleem tijdens het vinden van de langste match bij Knuts&&Pats\n");

  int start = 0;
  int gelijke_tekens = 0;
  int pos_eerste_mismach = 0;
  
  //Bepaal de verschuivings tabel.
  bereken_V(V,z,z_l);

  //Zet de lengtes gelijk aan elkaar
  p1->l = p2->l = 0;
  p1->p = p2->p = start; 

  if(z_l > t_l) z_l = t_l;
#ifdef lz77_debug
  printf("t_l: %d && z_l: %d\n", t_l, z_l);
  printf("t_l - z_l %d \n", t_l - z_l);

#endif

  while(start < z_l){
    //Bepaal de positie van de eerste mismatch
    pos_eerste_mismach = vergelijk_strings(z,t,z_l,t_l,start,gelijke_tekens);
    
    //Sla de start index op met basis 0 voor index vanaf sliding window
    p1->p = start;
    
    //De lengte van de match.
    p1->l = pos_eerste_mismach;
#ifdef lz77_debug
    printf("pos_eerste_mismatch: %d \n", pos_eerste_mismach);
#endif
    if(pos_eerste_mismach == z_l){ //we starten met tellen vanaf 0.
      //volledige sliding window is kunnen matchen, woehoe!!
      free(V);
      return;
    }
    
    //Check the size of the matches.
    if(p1->l > p2->l){
      //De nieuw gevonden lengte is groter dan de vorige
      p2->l = p1->l;
      p2->p = p1->p;
    }
    
    start += V[pos_eerste_mismach];
#ifdef lz77_debug
    printf("nieuwe start: %d \n", start);
    printf("V[pos_eerste_misMatch]: %d \n", V[pos_eerste_mismach]);
#endif
    if(pos_eerste_mismach == 0) gelijke_tekens = 0; //speciaal geval
    else gelijke_tekens = pos_eerste_mismach - V[pos_eerste_mismach];
  }
  free(V);
  
  //Als we hier zijn hebben we niet het volledige sliding window kunnen matchen
  //Wat meestal het geval zal zijn, de informatie over de langste match zit in p2
  //Het resultaat moet zitten in p1
  p1->l = p2->l;
  p1->p = p2->p;
}
