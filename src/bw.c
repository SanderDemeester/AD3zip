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
  partioneer(bwt_transformatie, rij_index, 0, blocksize);
  return NULL;
}
static void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

static void quicksort(char *rij, int* rij_index, int begin, int einde){
  if(begin >= einde) return;
  else if(begin + 10 > einde){
    //insertionsort
  }else{
    int spil_index = partioneer(rij, rij_index, begin,einde);
    quicksort(rij, rij_index, begin,spil_index-1);
    quicksort(rij, rij_index, spil_index+1, einde);
  }
}

static void printlist(char *rij, int *rij_index){
  int i = 0;
  for(; i < 4; i++){
    printf("%c \n", rij[rij_index[i]]);
  }
}
static int partioneer(char *rij, int *rij_index, int begin, int einde){
  int midden = (int)begin+einde/2;
  
  printf("%d \n", midden);
  
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
  printlist(rij,rij_index);

  return 0;
}

