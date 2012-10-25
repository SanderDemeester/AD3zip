#define MTF

typedef struct{
  unsigned char ascii_value;
  struct ascii_symbol *next;
  struct ascii_symbol *prev;
} ascii_symbol;
  

void move_to_front(char *string, int len);
