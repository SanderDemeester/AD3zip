#define MTF

/**************************************************************************************************/
/* Deze struct wordt gebruikt om alle tekens van extende ascii voor te stellen.			  */
/* Tijdens het gebruik van move to front zullen karakters regelmatig van plaats veranderen omdat  */
/* een karakter naar de eerste pos zal worden verplaatst.					  */
/**************************************************************************************************/
typedef struct{
  unsigned char ascii_value;
  struct ascii_symbol *next;
  struct ascii_symbol *prev;
} ascii_symbol;
  

void move_to_front(char *string, int len, int actie);
