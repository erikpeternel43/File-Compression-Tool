/* Min and max code length */

#ifndef MIN_CODE_LENGTH
#define MIN_CODE_LENGTH 9
#endif

#ifndef MAX_CODE_LENGTH
#define MAX_CODE_LENGTH 20
#endif

/* Max code written with MAX_CODE_LENGHT bits */
#ifndef MAX_CODE
#define MAX_CODE (1 << MAX_CODE_LENGTH)
#endif

/* Macro returns max number written by bits amount */
#ifndef CURRENT_MAX_CODES
#define CURRENT_MAX_CODES(bits) ((unsigned int)(1 << (bits)))
#endif

#ifndef LZW_H
#define LZW_H

/* Functions */
void lzw_encode(char *input, char *output);
void lzw_decode(char *input, char *output);

#endif