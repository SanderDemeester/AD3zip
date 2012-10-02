#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DECODEER 0
#define ENCODEER 1

int main(int argc, char* argv[]){
  int methode;
  int compressie_methode = 0;
  int blocksize = 0;
  if(argc < 4){
    fprintf(stderr, "Usage: %s [encodeer|decodeer] compressiemethode blocksize", argv[0]);
  }
  
  if(!strcmp(argv[1],"encodeer")) methode = ENCODEER;
  if(!strcmp(argv[1],"decodeer")) methode = DECODEER;
  
  return 0;
}

