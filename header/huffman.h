#define HUFFMAN
#include <stdint.h>

typedef struct{
  char *value;
  uint32_t weight;
  int aantal_elementen;
}huffman_top;

typedef struct{
  unsigned int code;
  int number_of_bits;
}huffman_codewoord;


void standaard_huffman(unsigned char *input_buffer, int lengte, int actie);

