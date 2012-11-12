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
static void quicksort(char *rij, int* rij_index, int begin, int einde, int len, int flag);

#ifdef DEBUG
static void printlist(char *rij, int *rij_index, int len);
#endif

void encoderen_bwt(char *bwt_block, int blocksize){

  char *bwt_transformatie = (char*) malloc(sizeof(char)*blocksize);
  int *rij_index = (int*) malloc(sizeof(int)*blocksize);
  int min_number_of_bytes = 1;
	   
  int i = 0;
  int flag = 1;

  memcpy((void*)bwt_transformatie, (void*)bwt_block, blocksize); //deep copy
  bwt_block = (char*) realloc(bwt_block, blocksize+2);

  for(i = 0; i < blocksize; i++) rij_index[i] = i;

  quicksort(bwt_transformatie, rij_index, 0,blocksize,blocksize,1); //lelijke hack

  #ifdef DEBUG
  printlist(bwt_transformatie,rij_index, blocksize);
  printf("-----------------\n");
  #endif

  bwt_block[4] = '_';

  for(i = 5; i < blocksize+5; i++){

    //printf("%c\n", *(&bwt_transformatie[(rij_index[i-2]+(blocksize-1)) % blocksize]));
    bwt_block[i] = *(&bwt_transformatie[(rij_index[i-5]+(blocksize-1)) % blocksize]);

    //printf("(%d,%d) - %c\n", i-2,blocksize-1,*(&bwt_transformatie[(rij_index[i-2]+(blocksize-1)) % blocksize]));
      if(bwt_block[i] == bwt_transformatie[0]){
	for(int j = 0; j < blocksize; j++){
	  // printf("%c | %c\n", *(&bwt_transformatie[(rij_index[i-2]+j-1) % blocksize]), bwt_transformatie[j]);
	  if(*(&bwt_transformatie[(rij_index[i-5]+j-1) % blocksize]) != bwt_transformatie[j]){
	    flag = 0;
	     break;
	  }
	}
	if(flag){
	   //	   printf("%s\n", first_pos);
	   while(min_number_of_bytes < 8 && i-5 >= (1 << (min_number_of_bytes*8))) min_number_of_bytes++;
	   /****************************************************************************************************************************************/
           /* Ipv -2 te doen (om de originele waarde te krijgen) doen we hier -1 ->+1, dit doen we om 0 te voorkomen tijdens het decoderen 	   */
           /****************************************************************************************************************************************/
	   //	   int t = (i-5)^0x10101010; //+1 to avoid null an
	   int t = i-5;
	   memcpy(bwt_block,&t,4); //copy byte over
	   //	   fwrite(bwt_block,1,4,stdout);
	   bwt_block[4] = '_';
	   //	   fwrite(bwt_block,1,5,stdout);
	}
	flag = 1; //Geef de andere nog een kans.
      }
  }
  if(bwt_block[0] > 255){
    fprintf(stderr,"assert error, bwt index vector size is greater then 255\n");
    exit(-1);
  }
  free(bwt_transformatie);
  free(rij_index);
}

void decoderen_bwt(char *bwt_vector, int len){

  //We gaan er vanuit dat de eerste 4 bytes de bwt index vector zijn.
  int start_pos = 0;
  memcpy(&start_pos,bwt_vector,4);

  /**************************************************************************************************/
  /* Omdat de blocksize in KiB is, moeten we rekening houden met grote indexen voor de bwt vector.  */
  /* We gebruiken dus fread, maar omdat we 4 bytes gebruiken kunnen de hogere bits gewoon \0 zijn.  */
  /* Dus daarom gebruiken we XOR, deze xor maken we hier terug ongedaan.			    */
  /**************************************************************************************************/
  start_pos = start_pos;//^0x10101010;

  //Rij van gesorteerde indexen.
  int *sorted_rij_index = (int*) malloc(sizeof(int)*len-1);
  int *bwt_rij_index    = (int*) malloc(sizeof(int)*len-1);
  
  char *temp = (char*) malloc(sizeof(char)*len);

  //Get maken van de array van indexen
  for(int i = 0; i < len; i++){
    sorted_rij_index[i] = i;
    bwt_rij_index[i] = i;
  }
  
  //we tellen 5 op bij de bwt_vector om de start pos van de '_' niet mee te sorteren.
  bwt_vector+=5;

  //sorteren van indexen van de bwt transformatie.
  quicksort(bwt_vector, sorted_rij_index, 0,len,len,0);

  #ifdef DECODE_DEBUG
  printf("De start pos is: %d \n", start_pos);
  #endif

  for(int i = 0; i < len; i++){

    #ifdef DEBUG
    printf("%c <-> %c\n", bwt_vector[sorted_rij_index[i]], bwt_vector[bwt_rij_index[i]]);
    #endif

    memcpy(&temp[i],&bwt_vector[start_pos],sizeof(char));
    start_pos = sorted_rij_index[bwt_rij_index[start_pos]];
  }

  memcpy(bwt_vector, temp,len);
  
  free(temp);
  free(sorted_rij_index);
  free(bwt_rij_index);
}

static void swap(int *a, int *b){
  int t = *a;
  *a = *b;
  *b = t;
}

static void quicksort(char *rij, int* rij_index, int begin, int einde, int len, int flag){  
  if(einde > begin + 1){
    char piv = rij[rij_index[begin]];
    int links = begin+1;
    int rechts = einde;
    while(links < rechts){
      if(rij[rij_index[links]] < piv){
	links++;
      }else if(rij[rij_index[links]] == piv && links != rechts){
	if(flag){
	  int offset = 1;
	  //	*(&rij[(rij_index[i]+j) % len]));
	  while(*(&rij[(rij_index[links]+offset)%len]) == *(&rij[(rij_index[begin]+offset)%len])){
	    if(offset+1 == len){	  
	      break;
	    } 
	    offset = (offset+1) % len;
	  }
	  
	  if(*(&rij[(rij_index[links]+offset) % len]) > *(&rij[(rij_index[begin]+offset) % len])){
	    swap(&(rij_index[links]),&(rij_index[--rechts]));
	  }else{
	    links++;	  
	  }
	}else{ //flag is off
	  if(rij_index[links] < rij_index[begin]){
	    links++;
	  }else{
	    swap(&(rij_index[links]),&(rij_index[--rechts]));
	  }
	}
      }else{
	swap(&rij_index[links],&rij_index[--rechts]);
      }
    }
    swap(&rij_index[--links] ,&rij_index[begin]);
    quicksort(rij,rij_index  ,begin,links,len,flag); 
    quicksort(rij,rij_index  ,rechts,einde,len,flag);
  }
}

#ifdef DEBUG
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
#endif
