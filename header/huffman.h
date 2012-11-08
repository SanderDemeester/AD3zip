#define HUFFMAN
#include <stdint.h>

typedef struct{
  char *value;
  uint32_t weight;
  int bin_value;
  struct huffman_top *links;
  struct huffman_top *rechts;
}huffman_top;


void standaard_huffman(char *input_buffer, int lengte, int actie);

