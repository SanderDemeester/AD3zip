#define COMPRESSIE_METHODE

typedef struct{
  int value;
  void (*compressie_algoritme)(unsigned char*,int,int);
}compressie_argument;

void not_implemented(unsigned char* input_buffer, int len, int actie);
void mtf_huffman(unsigned char *input_buffer, int len, int actie);
void semi_mtf(unsigned char *input_buffer, int len, int actie);
void debug(unsigned char *input_buffer, int len, int actie);

