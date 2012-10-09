#include "header/bw.h"
#include <math.h>
static void quicksort(char *rij, int* rij_index, int begin, int einde);
static int partioneer(char *rij, int *rij_inex, int begin, int einde);
static void printlist(char *rij, int *rij_index);

char *bwt(char *bwt_block, int blocksize){
  char *bwt_transformatie = (char*) malloc(sizeof(char)*blocksize);
  int *rij_index = (int*) malloc(sizeof(int)*blocksize);
  int i = 0;
  memcpy((void*)bwt_transformatie, (void*)bwt_block, blocksize); //deep copy
  for(i = 0; i < blocksize; i++) rij_index[i] = i;
  printlist(bwt_transformatie, rij_index);
  //  quicksort(bwt_transformatie, rij_index, 0,(sizeof(bwt_transformatie)/sizeof(char))-1);
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
  int j = 0;
  int aantal_element = sizeof(rij)/sizeof(char);
  for(i=0; i < aantal_element; i++){
    for(j = 0; j < aantal_element; j++){
      //printf("%c \n", rij[rij_index[i]]);
      printf("%c", *(&rij[rij_index[i+j] % aantal_element]));
    }
    printf("\n");
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
  while(rij[rij_index[links]] < spil || rij[rij_index[links]] == spil){
    if(rij[rij_index[links]] == spil && links != spil_index){
      int offset = 1;
#ifdef DEBUG
      printf("de pointer vanaf links is gelijk aan de spil\n");
      printf("volgende char om te vergelijken in de rij is: %c met index %d \n", *(&rij[rij_index[links]]+offset), links);
      printf("char volgende op de spil: %c met index %d \n", *(&rij[rij_index[spil_index]]+offset), spil_index);
#endif
														  
      while(*(&rij[rij_index[spil_index]]+offset) == *(&rij[rij_index[links]]+offset)){
#ifdef DEBUG
	printf("De volgende chars zijn nog altijd aan elkaar gelijk\n");
	printf("volgende char om te vergelijken in de rij is: %c\n", *(&rij[rij_index[links]]+offset));
	printf("char volgende op de spil: %c", *(&rij[rij_index[spil_index]]+offset));
#endif
	offset++ % sizeof(rij)/sizeof(char);
      }
      if(*(&rij[rij_index[spil_index]]+offset) > *(&rij[rij_index[spil_index]]+offset)){
#ifdef DEBUG
	printf("%c > %c\n", *(&rij[rij_index[spil_index]]+offset), *(&rij[rij_index[spil_index]]+offset));
#endif
	links++;
	break;
      }else{
#ifdef DEBUG
	printf("%c < %c\n", *(&rij[rij_index[spil_index]]+offset), *(&rij[rij_index[spil_index]]+offset));
#endif
	break;
      }
    }
#ifdef DEBUG
    printf("links->element in rij: %c < spil: %c \n", rij[rij_index[links]], spil);
#endif
    links++;
  }
  
  //laat rechts wijzen naar het eerste element kleiner dan de spil.
  while(rij[rij_index[rechts]] > spil || rij[rij_index[rechts]] == spil){
    if(rij[rij_index[rechts]] == spil && rechts != spil_index){
      printf("de pointer vanaf rechts is gelijk aan de spil");
      int offset = 1;
#ifdef DEBUG
      printf("de pointer vanaf rechts is gelijk aan de spil\n");
      printf("volgende char om te vergelijken in de rij is: %c met index %d \n", *(&rij[rij_index[links]]+offset), links);
      printf("char volgende op de spil: %c met index %d \n", *(&rij[rij_index[spil_index]]+offset), spil_index);
#endif
      while(*(&rij[rij_index[spil_index]]+offset) == *(&rij[rij_index[rechts]]+offset)){
	#ifdef DEBUG
	printf("De volgende chars zijn nog altijd aan elkaar gelijk\n");
	printf("volgende char om te vergelijken in de rij is: %c\n", *(&rij[rij_index[rechts]]+offset));
	printf("char volgende op de spil: %c", *(&rij[rij_index[spil_index]]+offset));
#endif
	offset++ % sizeof(rij)/sizeof(char);
      }
      if(*(&rij[rij_index[spil_index]]+offset) < *(&rij[rij_index[spil_index]]+offset)){
#ifdef DEBUG
	printf("%c < %c\n", *(&rij[rij_index[spil_index]]+offset), *(&rij[rij_index[spil_index]]+offset));
#endif
	rechts--;
	break;
      }else{
	break;
      }
    }
    #ifdef DEBUG
    printf("rechts->element in rij: %c > spil: %c \n", rij[rij_index[rechts]], spil);
    #endif
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
    if(rij[rij_index[links]] == spil){
      printf("found double match");
    }
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



