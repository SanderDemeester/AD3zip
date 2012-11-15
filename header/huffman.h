#define HUFFMAN
#include <stdint.h>

typedef struct{
  unsigned char *value;
  uint32_t weight;
  uint32_t aantal_elementen;
}huffman_top;

typedef struct{
  uint32_t code;
  uint32_t number_of_bits;
}huffman_codewoord;

//12 bytes
typedef struct{
  uint32_t huffman_block_len; // de lengte van het volledige huffman deel in bytes, zonder de header.
  uint32_t huffman_code_len;  // de lengte van het volledige huffman code deel in bytes.
  uint32_t huffman_boom_len;  // de lengte in bytes van de huffman boom.
}huffman_header;


void standaard_huffman(unsigned char *input_buffer, uint32_t lengte, uint32_t actie);

