#include "header/move_to_front.h"
#ifndef _GENERIC
#include "header/generic.h"
#endif

void move_to_front(char* string, int len){
  for(int i = 0; i < len; i++){
    fwrite(&string[i],1,sizeof(string[i]), stdout);
  }
}
