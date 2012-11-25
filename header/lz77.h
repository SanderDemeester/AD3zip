#include <stdint.h>
#define LZ77
#define G 6
#define LZ77_LEN_CODEWOORD 9


typedef struct{
  uint32_t p; //start positie in sliding window
  uint32_t l; //lengte van de match
  unsigned char x; //volgende byte die niet tot de match behoord.
}lz77_codewoord;

typedef struct{
  uint32_t p; //pos
  uint32_t l; //len
}match_pair;

typedef struct{
  unsigned char *res;
  uint32_t aantal_bytes;
}lz77_resultaat;

unsigned char* lz77_encodeer(unsigned char *input_buffer, int len);
lz77_resultaat* lz77_decoderen(unsigned char*input_buffer, int len);
void find_longest_match(unsigned char *t, int t_l, unsigned char *z, int z_l, match_pair* p1, match_pair* p2);
