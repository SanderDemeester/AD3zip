#define BW
void encoderen_bwt(char *bwt_block, int blocksize);
/*************************************************************************************/
/* De input van deze functie zal een bwt vector zijn.				     */
/* Deze zal nog moeten worden geparsed om de start positie in de vector te bepalen.  */
/*************************************************************************************/
void decoderen_bwt(char *bwt_vector, int len);
