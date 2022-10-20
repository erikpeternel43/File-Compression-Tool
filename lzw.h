/* Min and max code length */
#define MIN_CODE_LENGTH 9
#define MAX_CODE_LENGTH 20

#define MAX_CODE (1 << MAX_CODE_LENGTH)

/* Macro returns max number written by bits amount */
#define CURRENT_MAX_CODES(bits) ((unsigned int)(1 << (bits)))

/* Functions */
void lzw_encode(char *input, char *output);
void lzw_decode(char *input, char *output);
