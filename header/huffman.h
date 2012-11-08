#define HUFFMAN
#include <stdint.h>

typedef struct{
  char *value;
  uint32_t weight;
  int bin_value;
}huffman_top;


void standaard_huffman(char *input_buffer, int lengte, int actie);

