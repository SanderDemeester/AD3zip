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
static int partioneer(char *rij, int *rij_inex, int begin, int einde, int len);
static void printlist(char *rij, int *rij_index, int len);

void encoderen_bwt(char *bwt_block, int blocksize){
  char *bwt_transformatie = (char*) malloc(sizeof(char)*blocksize);
  int *rij_index = (int*) malloc(sizeof(int)*blocksize);
  int i = 0;
  memcpy((void*)bwt_transformatie, (void*)bwt_block, blocksize); //deep copy
  bwt_block = (char*) realloc(bwt_block, blocksize+2);
  for(i = 0; i < blocksize; i++) rij_index[i] = i;
  quicksort(bwt_transformatie, rij_index, 0,blocksize-1,blocksize);
  printlist(bwt_transformatie,rij_index, blocksize);
  printf("-----------------\n");
  bwt_block[1] = '_';
  for(i = 2; i < blocksize+2; i++){
    //    printf("%c\n", bwt_transformatie[rij_index[(i+(blocksize-1)) % blocksize]]);
    if(i-2 == 0){ 
      sprintf(bwt_block, "%d", rij_index[((i-2)+(blocksize-1)) % blocksize]);
      //      memcpy(bwt_block, buffer,1);
      bwt_block[1] = '_';
    }
    bwt_block[i] = bwt_transformatie[rij_index[((i-2)+(blocksize-1)) % blocksize]];
  }
}

void decoderen_bwt(char *bwt_vector, int len){
  //We gaan er vanuit dat het eerste element in de bwt vector de start pos is.
  int start_pos = atoi(&bwt_vector[0]);

  //Rij van indexen
  int *rij_index = (int*) malloc(sizeof(int)*len-1);

  //Get maken van de array van indexen
  for(int i = 0; i < len; i++) rij_index[i] = i;
  
  //we tellen 2 op bij de bwt_vector om de start pos van de '_' niet mee te sorteren.
  bwt_vector+=2;
  
  quicksort(bwt_vector, rij_index, 0,len-1,len);
  for(int i = 0; i < len; i++){
    printf("%c", bwt_vector[rij_index[i]]);
  }
  printf("\n");
  
}

static void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

static void quicksort(char *rij, int* rij_index, int begin, int einde, int len){
  int spil_index = 0; 
  if(begin >= einde) return; //stop
  spil_index = partioneer(rij, rij_index, begin,einde,len);
#ifdef DEBUG
  printf("execute 1\n");
#endif
  quicksort(rij, rij_index, begin,spil_index-1,len);
#ifdef DEBUG
  printf("execute 2\n");
#endif
  quicksort(rij, rij_index, spil_index+1, einde, len);
  
}

static void printlist(char *rij, int *rij_index, int len){
  int i = 0;
  int j = 0;
  for(i=0; i < len; i++){
    for(j = 0; j < len; j++){
      //printf("%c \n", rij[rij_index[i]]);
      printf("%c", *(&rij[rij_index[(i+j) % len]]));
    }
    printf("\n");
  }
}
static int partioneer(char *rij, int *rij_index, int begin, int einde, int len){
  int aantal = len;
  int midden = (begin+(einde-1))/2; 
  int spil_index = 0;
  char spil = 0;
  int links = 0;
  int rechts = 0;


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
  
  
  spil_index = midden; //het middelste element is de index van de spil.
  spil = rij[rij_index[midden]]; //het spil element.
  swap(&rij_index[einde],&rij_index[spil_index]); //wissel de index van het laatste element in de rij met de index van het spil element.
  links = begin; //links is gelijk aan het begin van de rij index
  rechts = einde-1; //begin te testen van het einde 

#ifdef DEBUG
  printf("einde: %d \n", einde);
  printf("spil element is: %c \n", spil);
  printlist(rij,rij_index, len);
  printf("------------\n");
#endif

  //laat links wijzen naar het eerste element groter dan de spil
  while(rij[rij_index[links]] < spil || rij[rij_index[links]] == spil){
    if(rij[rij_index[links]] == spil && links != spil_index){
      int offset = 1;
#ifdef DEBUG
      printf("de pointer vanaf links is gelijk aan de spil, de spil is: %c = %c \n",spil);
      printf("volgende char om te vergelijken in de rij (vanaf links) is: %c met index %d \n", *(&rij[rij_index[(links+offset) % aantal]]),links);
      printf("char volgende op de spil: %c met index %d \n", *(&rij[rij_index[(spil_index+offset) % aantal]]), spil_index);
#endif
														  
      while(*(&rij[rij_index[(spil_index+offset) % aantal]]) == *(&rij[rij_index[(links+offset) % aantal]])){
#ifdef DEBUG
	printf("De volgende chars zijn nog altijd aan elkaar gelijk\n");
	printf("volgende char om te vergelijken (vanaf links) in de rij is: %c\n", *(&rij[rij_index[(links+offset) % aantal]]));
	printf("volgende char om te vergelijken (vanaf spil)  in de rij is:  %c",  *(&rij[rij_index[(spil_index+offset) % aantal]]));
#endif
	offset = (offset+1) % aantal;
      }
      if(*(&rij[rij_index[(links+offset) % aantal]]) > *(&rij[rij_index[(spil_index+offset) % aantal]])){
#ifdef DEBUG
	printf("%c > %c\n", *(&rij[rij_index[spil_index+offset] % aantal]), *(&rij[rij_index[(spil_index+offset) % aantal]]));
#endif
	links++;
	break;
      }else{
#ifdef DEBUG
	printf("|%c < %c\n", *(&rij[rij_index[(links+offset) % aantal]]), *(&rij[rij_index[(spil_index+offset) % aantal]]));
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
#ifdef DEBUG
      printf("de pointer vanaf rechts is gelijk aan de spil");
#endif
      int offset = 1;
#ifdef DEBUG
      printf("de pointer vanaf rechts is gelijk aan de spil\n");
      printf("volgende char om te vergelijken in de rij is: %c met index %d \n", *(&rij[rij_index[links]]+offset), links);
      printf("char volgende op de spil: %c met index %d \n", *(&rij[rij_index[spil_index]]+offset), spil_index);
#endif
      while(*(&rij[rij_index[(spil_index+offset) % aantal]]) == *(&rij[rij_index[(rechts+offset) % aantal]])){
	#ifdef DEBUG
	printf("De volgende chars zijn nog altijd aan elkaar gelijk\n");
	printf("volgende char om te vergelijken in de rij is: %c\n", *(&rij[rij_index[(rechts+offset)%aantal]]));
	printf("char volgende op de spil: %c", *(&rij[rij_index[(spil_index+offset) % aantal]]));
#endif
	offset = (offset+1) % aantal;
      }
      if(*(&rij[rij_index[(rechts+offset) % aantal]]) < *(&rij[rij_index[(spil_index+offset) % aantal]])){
#ifdef DEBUG
	printf("%c < %c\n", *(&rij[rij_index[(spil_index+offset) % aantal]]), *(&rij[rij_index[(spil_index+offset) % aantal]]));
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
  printlist(rij,rij_index,len);
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
    printlist(rij,rij_index, len);
#endif
    swap(&rij_index[links],&rij_index[rechts]);
#ifdef DEBUG
    printf("na swap\n");
    printlist(rij,rij_index,len);
    printf("------------\n");
    if(rij[rij_index[links]] == spil){
      printf("found double match");
    }
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
  printlist(rij,rij_index, aantal);
  #endif
  return spil_index;
}



