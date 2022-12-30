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
    int curr_char;
    int curr_char_pos; 
    unsigned char * buffer;
    size_t buffer_pos;
    size_t buffer_strlen;

} FileStream;

/* Functions */
FileStream* open_file_stream(char *fileName, Mode mode, int curr_char, int curr_char_pos);
void close_file_stream(FileStream *stream);
void clear_output_buffer(FileStream *stream);
int read_byte(FileStream *stream);
int read_bit(FileStream *stream);
void write_code(FileStream *stream, char *code, unsigned int code_length);
void write_byte(FileStream *stream, unsigned char byte);
int get_char_from_buffer(FileStream *stream);
void put_char_to_buffer(FileStream *stream, unsigned char character);

#endif
