#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DECODEER 0
#define ENCODEER 1

int main(int argc, char* argv[]){
  int methode;
  int compressie_methode = 0;
  int blocksize = 0;
  char *input_buffer = NULL; //input buffer
  char c;

  ssize_t input_lenth = 0; //default input len
  if(argc < 4){
    fprintf(stderr, "Usage: %s [encodeer|decodeer] compressiemethode blocksize", argv[0]);
  }
  
  if(!strcmp(argv[1],"encodeer")) methode = ENCODEER;
  if(!strcmp(argv[1],"decodeer")) methode = DECODEER;
  compressie_methode = atoi(argv[2]);
  blocksize = atoi(argv[3]);
  
  while((fread(&c,1,1,stdin))) {
    printf("%c", c);
  }
  
  return 0;
}

