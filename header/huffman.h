#define HUFFMAN
#include <stdint.h>

#define HUFFMAN_HEADER_SIZE 12

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

typedef struct{
  int is_root:1;
  int bit:1;
  int is_blad:1;
  char *value;
  struct huffman_tree_element* links;
  struct huffman_tree_element* rechts;
}huffman_tree_element;

typedef struct{
  unsigned char* res;
  int aantal_bytes;
}huffman_decode_result;

huffman_decode_result * standaard_huffman(unsigned char *input_buffer, uint32_t lengte, uint32_t actie);

void build_huffmancode(huffman_top **huffman_toppen,
		       huffman_codewoord **code,
		       int number_of_huffman_top);

				 

