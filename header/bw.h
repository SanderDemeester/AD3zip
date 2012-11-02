#define BW
/***************************************************************************************/
/* De input van deze functie zal het adr zijn van een string met lengte blocksize die  */
/* moet worden omgevormt naar een bwt vector					       */
/***************************************************************************************/
void encoderen_bwt(char *bwt_block, int blocksize);

/*************************************************************************************/
/* De input van deze functie zal een bwt vector zijn.				     */
/* Deze zal nog moeten worden geparsed om de start positie in de vector te bepalen.  */
/*************************************************************************************/
void decoderen_bwt(char *bwt_vector, int len);
