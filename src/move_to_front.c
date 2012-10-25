#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

void move_to_front(char* string, int len){
  printf("begin move_to_front\n");
  unsigned char index = 0;
  unsigned char *alpha = (unsigned char*) malloc(sizeof(unsigned char)*255);
  for(; index < (unsigned char) 255; index++){
    alpha[index] = index;
  }
  if(string[0] == alpha[49]){
    printf("ok\n");
  }
  printf("end move to front\n");
}
