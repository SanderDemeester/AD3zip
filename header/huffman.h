#define HUFFMAN
#include <stdint.h>

typedef struct{
  char *value;
  uint32_t weight:31;
  uint32_t bit:1;
}huffman_blad;

void standaard_huffman(char *input_buffer, int lengte, int actie);
