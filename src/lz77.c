#include "header/lz77.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

void lz77_encodeer(unsigned char *input_buffer, int len){
  int start_sliding_window = 0;
  int end_sliding_window = 0;

  uint32_t index_huidig_element = 0;
  uint32_t code_woorde_index = 0;

  lz77_codewoord **codewoorden = (lz77_codewoord*) calloc(1, sizeof(lz77_codewoord*));

  match_pair *p1 = (match_pair*) calloc(1,sizeof(match_pair));
  match_pair *p2 = (match_pair*) calloc(1,sizeof(match_pair));

  while(index_huidig_element < len && end_sliding_window < len){
    
    find_longest_match(input_buffer+(end_sliding_window+1), //het eerste volgende byte volgend op het einde van het sliding window
		       len-(end_sliding_window+1), //de volledige lengte van de buffer - de lengte van het sliding window
		       input_buffer + start_sliding_window, //start van sliding window
		       end_sliding_window - start_sliding_window,
		       p1,
		       p2);
		       
    end_sliding_window++;
    
    if((end_sliding_window - start_sliding_window) >= G){
	printf("hier\n");
      start_sliding_window++;
    }
    printf("sliding window index, start: %d && end: %d \n", start_sliding_window, end_sliding_window);
    index_huidig_element++;
    printf("len: %d \n", len);
    
  }

  codewoorden[0] = (lz77_codewoord*) calloc(1,sizeof(lz77_codewoord));
  free(codewoorden[0]);
  free(codewoorden);
  free(p1);
  free(p2);
}

//return pos eerste mis_match.
static int vergelijk_strings(unsigned char *z, unsigned char*t, int z_l, int t_l, int start, int gelijke_tekens){
  for(int i = 0; i < z_l; i++)
    if(z[i] != t[start+i]) return i;

  return z_l;
}
static void bereken_V(int *V, unsigned char *z, uint32_t z_l){
  //z_l = 3-> {0,1,2}
  int start = 1;
  int gelijke_tekens = 0;
  int pos_eerste_mismach = 0;

  V[0] = 1;
  V[1] = 1;
  
  while(start < (z_l-1)){
    pos_eerste_mismach = vergelijk_strings(z,z,z_l,z_l,start,gelijke_tekens);
    if(pos_eerste_mismach == 0) V[start+1] = start+1;
    else
      for(int i = gelijke_tekens+1; i <= pos_eerste_mismach; i++) 
	V[start+i] = start;
    start += start+V[pos_eerste_mismach];
    if(pos_eerste_mismach == 0) gelijke_tekens = 0; 
    else gelijke_tekens = pos_eerste_mismach-V[pos_eerste_mismach];
  }
  
  
  
  
}
void find_longest_match(unsigned char *t, uint32_t t_l, 
			    unsigned char *z, uint32_t z_l,
			    match_pair *p1,
			    match_pair *p2){

  int *V = (int*) calloc(z_l, sizeof(int)); 
  int start = 0;
  int gelijke_tekens = 0;
  int pos_eerste_mismach = 0;
  
  //Bepaal de verschuivings tabel.
  bereken_V(V,z,t_l);

  //Zet de lengtes gelijk aan elkaar
  p1->l = p2->l = 0;
  p1->p = p2->p = start; 
  
  if(t_l - z_l < 0){
    int t = t_l;
    unsigned char *b = t;
    t_l = z_l;
    z_l = t;
    
    t = z;
    z = b;
  }

  
  printf("t_l: %d && z_l: %d", t_l, z_l);
  printf("%d \n", t_l - z_l);

  while(start <= (t_l - z_l)){
    //Bepaal de positie van de eerste mismatch
    pos_eerste_mismach = vergelijk_strings(z,t,z_l,t_l,start,gelijke_tekens);
    
    //Sla de start index op met basis 0 voor index vanaf sliding window
    p1->p = start;
    
    //De lengte van de match.
    p1->l = pos_eerste_mismach;
    
    printf("pos_eerste_mismatch: %d \n", pos_eerste_mismach);
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
