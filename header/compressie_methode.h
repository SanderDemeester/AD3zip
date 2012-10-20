#define COMPRESSIE_METHODE

typedef struct{
  int value;
  void (*compressie_algoritme)(int,int);
}compressie_argument;

void not_implemented(int x, int y);
void mtf_huffman(int x, int y);
void semi_mtf(int x, int y);

