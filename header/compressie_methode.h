#define COMPRESSIE_METHODE

typedef struct{
  int value;
  void (*compressie_algoritme)(int,int);
}compressie_argument;

void not_implemented(char* input_buffer, int len);
void mtf_huffman(char *input_buffer, int len);
void semi_mtf(char *input_buffer, int len);
void debug(char *input_buffer, int len);

