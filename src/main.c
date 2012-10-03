#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DECODEER 0
#define ENCODEER 1

int main(int argc, char* argv[]){
  int methode = 0; //default methode is decodeer
  int compressie_methode = 0;
  int blocksize = 0;
  char *input_buffer = (char*) malloc(sizeof(char));
  char *t = NULL;
  char c = NULL;
  int input_lengte = 0; 
  
  if(argc < 4 && (argc == 2 && strcmp(argv[1],"decodeer")) || argc==1){
    fprintf(stderr, "Usage: %s [encodeer|decodeer] compressiemethode blocksize", argv[0]);
    exit(-1);
  }

  if(!strcmp(argv[1],"encodeer")) methode = ENCODEER;
  if(!strcmp(argv[1],"decodeer")) methode = DECODEER;
  
  if(methode){
    compressie_methode = atoi(argv[2]);
    blocksize = atoi(argv[3]);
  }
  
  while((fread(&c,1,1,stdin))) {
    input_lengte++;
    t = (char*) realloc(input_buffer,sizeof(char));
    if(t != NULL){
      input_buffer = t;
      input_buffer[input_lengte-1] = c;
    }else{
      free(input_buffer);
      fprintf(stderr,"Fout tijdens allocteren van input buffer");
      exit(-1);
    }
  }
    
  if(ENCODEER){
  /***************************************************************/
  /* input_buffer is de buffer die input tekens van stdin bevat	 */
  /* input_lengte is het aantal tekens dat input_buffer bevat	 */
  /* input_block  is de buffer die de tekens van de buffer bevat */
  /***************************************************************/
    
    char *input_block = (char*) malloc(sizeof(char)*blocksize);
#ifdef DEBUG
    printf("Encodeer\n");
    printf("%d \n", input_lengte);
#endif
    while(input_lengte){
      if(input_lengte < blocksize){
	blocksize = input_lengte;
      }
      memcpy((void*)input_block, (void*) input_buffer, blocksize);
      printf("%s", input_block);
      input_lengte -= blocksize; //subtract from input_lengte
      input_buffer += blocksize; //add to input_buffer
    }
  }
  return 0;
}
