#include "header/bw.h"
#include <math.h>
static void quicksort(char *rij, int* rij_index, int begin, int einde);
static int partioneer(char *rij, int *rij_inex, int begin, int einde);


char *bwt(char *bwt_block, int blocksize){
  char *bwt_transformatie = (char*) malloc(sizeof(char)*blocksize);
  int *rij_index = (int*) malloc(sizeof(int)*blocksize);
  int i = 0;
  memcpy((void*)bwt_transformatie, (void*)bwt_block, blocksize); //deep copy
  for(i = 0; i < blocksize; i++) rij_index[i] = i;

  quicksort(bwt_transformatie, rij_index, 0,blocksize-1);
  //  int spil = partioneer(bwt_transformatie, rij_index, 0, blocksize);
  return NULL;
}
static void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

static void quicksort(char *rij, int* rij_index, int begin, int einde){
  if(begin >= einde) return; //stop
  /***************************************/
  /* else if(begin + 10 > einde){	 */
  /*   #ifdef DEBUG			 */
  /*   printf("voer insertionsort uit"); */
  /*   #endif				 */
  /*   //insertionsort			 */
  /* }else{				 */
  /***************************************/
    int spil_index = partioneer(rij, rij_index, begin,einde);
    #ifdef DEBUG
    printf("execute 1\n");
    #endif
    quicksort(rij, rij_index, begin,spil_index-1);
    #ifdef DEBUG
    printf("execute 2\n");
    #endif
    quicksort(rij, rij_index, spil_index+1, einde);
    
    //  }
}

static void printlist(char *rij, int *rij_index){
  int i = 0;
  for(; i < 4; i++){
    printf("%c \n", rij[rij_index[i]]);
  }
}
static int partioneer(char *rij, int *rij_index, int begin, int einde){

  int midden = (begin+(einde-1))/2; 

  #ifdef DEBUG
  printf("begin:  %d \n", begin);
  printf("midden: %d \n", midden);
  printf("einde:  %d \n", einde);
  #endif
  

  //Mediaan van 3
  if(rij[rij_index[begin]] > rij[rij_index[midden]]){
    #ifdef DEBUG
    printf("swap1 \n");
    #endif
    swap(&rij_index[begin],&rij_index[midden]);
  }
  if(rij[rij_index[midden]] > rij[rij_index[einde]]){
    #ifdef DEBUG
    printf("swap2 \n");
    #endif
    swap(&rij_index[midden],&rij_index[einde]);
  }
  if(rij[rij_index[einde]] < rij[rij_index[begin]]){
    #ifdef DEBUG
    printf("swap3 \n");
    #endif 
    swap(&rij_index[einde],&rij_index[begin]);
  }
  
  
  int spil_index = midden; //het middelste element is de index van de spil.
  char spil = rij[rij_index[midden]]; //het spil element.
  swap(&rij_index[einde],&rij_index[spil_index]); //wissel de index van het laatste element in de rij met de index van het spil element.
  int links = begin; //links is gelijk aan het begin van de rij index
  int rechts = einde-1; //begin te testen van het einde 

#ifdef DEBUG
  printf("einde: %d \n", einde);
  printf("spil element is: %c \n", spil);
  printlist(rij,rij_index);
  printf("------------\n");
#endif
  //laat links wijzen naar het eerste element groter dan de spil
  while(rij[rij_index[links]] < spil){
    printf("links->element in rij: %c < spil: %c \n", rij[rij_index[links]], spil);
    links++;
  }
  //laat rechts wijzen naar het eerste element kleiner dan de spil.
  while(rij[rij_index[rechts]] > spil){
    printf("rechts->element in rij: %c > spil: %c \n", rij[rij_index[rechts]], spil);
    rechts--;
  }
  
#ifdef DEBUG
  printf("------------\n");
  printf("links: %d \n", links);
  printf("rechts: %d \n", rechts);
  printlist(rij,rij_index);
  printf("------------\n");
#endif
  
  //zolang de pointers elkaar overlappen moeten we wissel acties uitvoeren.
  while(links < rechts){
    #ifdef DEBUG
    printf("execute while links < rechts\n");
    printf("------------\n");
    printf("links: %d \n", links);
    printf("rechts: %d \n", rechts);
    printf("------------\n");
    printf("voor swap \n");
    printlist(rij,rij_index);
    #endif
    swap(&rij_index[links],&rij_index[rechts]);
    #ifdef DEBUG
    printf("na swap\n");
    printlist(rij,rij_index);
    printf("------------\n");
    #endif
    while(rij[rij_index[links]] < spil) links++;
    while(rij[rij_index[rechts]] > spil) rechts--;
  }
  
  spil_index = links; //links wijst naar de positie vlak voor het element dat groter is dan de spil.
  #ifdef DEBUG
  printf("nieuwe spil_index element is: %d \n", spil_index);
  #endif
  swap(&rij_index[spil_index],&rij_index[einde]);
  
  #ifdef DEBUG
  printlist(rij,rij_index);
  #endif
  return spil_index;
}



