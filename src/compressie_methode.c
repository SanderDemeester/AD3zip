#include "header/compressie_methode.h"
#include <stdio.h>
void not_implemented(char *input_buffer, int len){
  printf("Not implemented, options 2 and 4.\n");
}
void mtf_huffman(char *input_buffer, int len){
  #ifdef DEBUG
  printf("Move to Front in combinatie met standaard huffman, optie 1\n");
  #endif
  move_to_front(input_buffer, len);
}
void semi_mtf(char *input_buffer, int len){
  printf("Semi move to front in combinatie met standaard huffman, optie 3\n");
}
void debug(char *input_buffer, int len){
  for(int i = 0; i < len; i++){
    fwrite(&input_buffer[i],1,sizeof(input_buffer[i]),stdout);
  }
}
