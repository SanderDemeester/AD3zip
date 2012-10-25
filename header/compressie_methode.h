#define COMPRESSIE_METHODE

typedef struct{
  int value;
  void (*compressie_algoritme)(int,int,int);
}compressie_argument;

void not_implemented(char* input_buffer, int len, int actie);
void mtf_huffman(char *input_buffer, int len, int actie);
void semi_mtf(char *input_buffer, int len, int actie);
void debug(char *input_buffer, int len, int actie);

