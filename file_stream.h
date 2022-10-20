/* Included files */
#include <stdio.h>

/* Modes for opening file */
typedef enum
{
    FILE_READ = 0,    
    FILE_WRITE = 1,   
    FILE_APPEND = 2,   
} Mode;

/* Structure for files that are currently opened */
typedef struct
{
    FILE *fp;                  
    Mode mode;   
    int buf;
    int bufpos; 
} FileStream;

/* Functions */
FileStream *open_file_stream(char *fileName, Mode mode, int buf, int bufpos);
void close_file_stream(FileStream *stream);
void write_code(FileStream *stream, unsigned int code, unsigned int code_length);
int read_code(FileStream *stream, unsigned int code_length);


