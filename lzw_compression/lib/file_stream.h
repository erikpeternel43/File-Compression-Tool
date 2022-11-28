#include <stdio.h>
#include <stdbool.h>

#ifndef FILE_STREAM_H
#define FILE_STREAM_H

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
    int buffer;
    int buffer_pos; 
} FileStream;

/* Functions */
FileStream* open_file_stream(char *fileName, Mode mode, int buffer, int buffer_pos);
void close_file_stream(FileStream *stream);
void clear_output_buffer(FileStream *output_stream);
void write_code(FileStream *stream, unsigned int code, unsigned int code_length);
void write_byte(FileStream *stream, unsigned char byte);
bool read_code(FileStream *stream, unsigned int *code, unsigned int code_length);

#endif
