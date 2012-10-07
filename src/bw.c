#include "header/bw.h"
static void quicksort(char *rij, int* rij_index, int begin, int einde);
static int partioneer(char *rij, int *rij_inex, int begin, int einde);


char *bwt(char *bwt_block, int blocksize){
  char *bwt_transformatie = (char*) malloc(sizeof(char)*blocksize);
  int *rij_index = (int*) malloc(sizeof(int)*blocksize);
  int i = 0;
  memcpy((void*)bwt_transformatie, (void*)bwt_block, blocksize); //deep copy
  for(i = 0; i < blocksize; i++) rij_index[i] = i;

  //  quicksort(bwt_transformatie, rij_index, 0,blocksize);
  int spil = partioneer(bwt_transformatie, rij_index, 0, blocksize);
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
    printf("execute 1");
    #endif
    quicksort(rij, rij_index, begin,spil_index-1);
    #ifdef DEBUG
    quicksort(rij, rij_index, spil_index+1, einde);
    #endif
    
    //  }
}

static void printlist(char *rij, int *rij_index){
  int i = 0;
  for(; i < 4; i++){
    printf("%c \n", rij[rij_index[i]]);
  }
}
static int partioneer(char *rij, int *rij_index, int begin, int einde){
  einde--;
  int midden = (int)begin+einde/2;
  
  #ifdef DEBUG
  printf("midden: %d \n", midden);
  #endif
  
  if(rij[rij_index[begin]] > rij[rij_index[midden]]){
    printf("swap1 \n");
    swap(&rij_index[begin],&rij_index[midden]);
  }
  if(rij[rij_index[midden]] > rij[rij_index[einde]]){
    printf("swap2 \n");
    swap(&rij_index[midden],&rij_index[einde]);
  }
  if(rij[rij_index[einde]] < rij[rij_index[begin]]){
    printf("swap3 \n");
    swap(&rij_index[einde],&rij_index[begin]);
  }
  

  
  int spil_index = midden;
  char spil = rij[rij_index[midden]];
  swap(&rij_index[einde],&rij_index[spil_index]);
  int links = begin;
  int rechts = einde-1;

#ifdef DEBUG
  printf("spil element is: %c \n", spil);
#endif
  while(rij[rij_index[++links]] < spil);
  while(rij[rij_index[--rechts]] > spil);
  
  
  while(links < rechts){
    swap(&rij_index[links],&rij_index[rechts]);
    while(rij[rij_index[++links]] < spil);
    while(rij[rij_index[--rechts]] > spil);
  }
  
  spil_index = links;
  swap(&rij_index[spil_index],&rij_index[einde-1]);

  #ifdef DEBUG
  printlist(rij,rij_index);
  #endif
  return spil_index;
}

