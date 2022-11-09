/* Min and max code length */

#ifndef MIN_CODE_LENGTH
#define MIN_CODE_LENGTH 9
#endif

#ifndef MAX_CODE_LENGTH
#define MAX_CODE_LENGTH 20
#endif

#ifndef MAX_CODE
#define MAX_CODE (1 << MAX_CODE_LENGTH)
#endif

/* Macro returns max number written by bits amount */
#ifndef CURRENT_MAX_CODES
#define CURRENT_MAX_CODES(bits) ((unsigned int)(1 << (bits)))
#endif

#ifndef lzw
#define lzw

/* Functions */
void lzw_encode(char *input, char *output);
void lzw_decode(char *input, char *output);

#endif