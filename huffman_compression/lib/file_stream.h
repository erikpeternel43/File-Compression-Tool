#include <stdio.h>

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
int read_byte(FileStream *stream);
int read_bit(FileStream *stream);
void write_code(FileStream *stream, char *code, unsigned int code_length);
void write_byte(FileStream *stream, unsigned char byte);
int get_char(FileStream *stream);

#endif
