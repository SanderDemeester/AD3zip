#include "header/bw.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

/**********************************************************************************************************************************/
/* Het eerste argument is een pointer naar de rij van karakters.								  */
/* Het tweede argument is een pointer naar de rij van indexen van de karakters							  */
/* Het derde argument is het begin van de rij, voor de eerste oproep 0								  */
/* Het vierde argument is de index van het laatste element van de rij. BV: als er 4 elementen zijn 0,1,2,3 dan is dit argument 3. */
/* Het vijfde argument is het aantal elementen in de rij. Bij het voorbeeld hierboven zijn dit dus 4 zijn.  			  */
/**********************************************************************************************************************************/
static void quicksort(char *rij, int* rij_index, int begin, int einde, int len);
static void printlist(char *rij, int *rij_index, int len);

void encoderen_bwt(char *bwt_block, int blocksize){
  char *bwt_transformatie = (char*) malloc(sizeof(char)*blocksize);
  int *rij_index = (int*) malloc(sizeof(int)*blocksize);
  int i = 0;
  memcpy((void*)bwt_transformatie, (void*)bwt_block, blocksize); //deep copy
  bwt_block = (char*) realloc(bwt_block, blocksize+2);
  for(i = 0; i < blocksize; i++) rij_index[i] = i;
  quicksort(bwt_transformatie, rij_index, 0,blocksize,blocksize);
  #ifdef DEBUG
  printlist(bwt_transformatie,rij_index, blocksize);
  printf("-----------------\n");
  #endif
  bwt_block[1] = '_';
  for(i = 2; i < blocksize+2; i++){
    //    printf("%c\n", bwt_transformatie[rij_index[(i+(blocksize-1)) % blocksize]]);
    if(i-2 == 0){ 
      //      printf("%d \n", abs(rij_index[0]-1));
      sprintf(bwt_block, "%d", abs(rij_index[0]-1));
      //      memcpy(bwt_block, buffer,1);
      bwt_block[1] = '_';
    }
    bwt_block[i] = *(&bwt_transformatie[(rij_index[i-2]+(blocksize-1)) % blocksize]);
    //    printf("(%d,%d) - %c\n", i-2,blocksize-1,*(&bwt_transformatie[(rij_index[i-2]+(blocksize-1)) % blocksize]));
    
  }
}

void decoderen_bwt(char *bwt_vector, int len){
  //We gaan er vanuit dat het eerste element in de bwt vector de start pos is.
  int start_pos = atoi(&bwt_vector[0]);
  
  //We houden de huidige positie bij tijdens het decoderen.
  int print_index = start_pos;
  //Rij van gesorteerde indexen.
  int *sorted_rij_index = (int*) malloc(sizeof(int)*len-1);
  int *bwt_rij_index    = (int*) malloc(sizeof(int)*len-1);


  //Get maken van de array van indexen
  for(int i = 0; i < len; i++){
    sorted_rij_index[i] = i;
    bwt_rij_index[i] = i;
  }
  
  //we tellen 2 op bij de bwt_vector om de start pos van de '_' niet mee te sorteren.
  bwt_vector+=2;

  //sorteren van indexen van de bwt transformatie.
  quicksort(bwt_vector, sorted_rij_index, 0,len,len);
  
  #ifdef DECODE_DEBUG
  printf("De start pos is: %d \n", start_pos);
  #endif
  for(int i = 0; i < len; i++){
    #ifdef DECODE_DEBUG
    printf("%c <-> %c\n", bwt_vector[sorted_rij_index[i]], bwt_vector[bwt_rij_index[i]]);
    #endif
    printf("%c", bwt_vector[start_pos]);

    start_pos = sorted_rij_index[bwt_rij_index[start_pos]];

  }
  printf("\n");
  free(sorted_rij_index);
  free(bwt_rij_index);
  
}

static void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

static void quicksort(char *rij, int* rij_index, int begin, int einde, int len){
  if(einde > begin + 1){
    char piv = rij[rij_index[begin]];
    int links = begin+1;
    int rechts = einde;
    while(links < rechts){
      if(rij[rij_index[links]] < piv){
	links++;
      }else if(rij[rij_index[links]] == piv){
	int offset = 1;
	while(*(&rij[rij_index[(links+offset) % len]]) == *(&rij[rij_index[(begin+offset) % len]])){
	  if(offset+1 == len) break;
	  offset = (offset+1) % len;
	}
	if(*(&rij[rij_index[(links+offset) % len]]) > *(&rij[rij_index[(begin+offset) % len]])){
	  swap(&rij_index[links],&rij_index[--rechts]);
	}else{
	  links++;
	}
      }else{
	swap(&rij_index[links],&rij_index[--rechts]);
      }
    }
    swap(&rij_index[--links] ,&rij_index[begin]);
    quicksort(rij,rij_index  ,begin,links,len);
    quicksort(rij,rij_index  ,rechts,einde,len);
  }
}

static void printlist(char *rij, int *rij_index, int len){
  int i = 0;
  int j = 0;
  for(i=0; i < len; i++){
    for(j = 0; j < len; j++){
      //printf("%c \n", rij[rij_index[i]]);
      //Deze structuur gaat mij dus echt blijven achtervolgen
      printf("(%d,%d) - %c", i,j,*(&rij[(rij_index[i]+j) % len]));
    }
    printf("\n");
  }
}



 
