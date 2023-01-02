#include <stdio.h>
#include <stdbool.h>

#ifndef IO_BUFFER_SIZE
#define IO_BUFFER_SIZE 1024
#endif

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
    int char_buffer;
    int char_buffer_pos; 
    unsigned char * io_buffer;
    size_t io_buffer_pos;
    size_t io_buffer_length;

} FileStream;

/* Functions */
FileStream* open_file_stream(char *fileName, Mode mode, int char_buffer, int char_buffer_pos);
void close_file_stream(FileStream *stream);
void clear_output_buffer(FileStream *stream);
void write_code(FileStream *stream, unsigned int code, unsigned int code_length);
bool read_code(FileStream *stream, unsigned int *code, unsigned int code_length);
int get_char(FileStream *stream);
void put_char(FileStream *stream, unsigned char character);


#endif
