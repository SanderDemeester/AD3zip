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

typedef struct{
  unsigned int huffman_block_len; // de lengte van het volledige huffman deel in bytes
  unsigned int huffman_code_len;  // de lengte van het volledige huffman code deel in bytes
  unsigned int huffman_boom_len;  // de lengte in bytes van de huffman boom
}huffman_header;


void standaard_huffman(unsigned char *input_buffer, int lengte, int actie);

