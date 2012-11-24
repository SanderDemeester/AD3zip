#define COMPRESSIE_METHODE
#define BWT_HEADER_LEN 5

typedef struct{
  int value;
  void (*compressie_algoritme)(unsigned char*,int,int,int);
}compressie_argument;

void not_implemented(unsigned char* input_buffer, int len, int actie,int blocksize);
void mtf_huffman(unsigned char *input_buffer, int len, int actie,int blocksize);
void semi_mtf(unsigned char *input_buffer, int len, int actie,int blocksize);
void lz77(unsigned char *input_buffer, int len, int actie,int blocksize);
void debug(unsigned char *input_buffer, int len, int actie,int blocksize);

