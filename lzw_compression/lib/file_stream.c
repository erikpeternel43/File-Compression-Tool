#include "file_stream.h"
#include <stdlib.h>
#include <stdbool.h>
#include "err_sys.h"

/* Additional functions */
static int next_bit(FileStream *stream);

/* Function initialize and returns FileStream structure */
FileStream *open_file_stream(char *fileName, Mode mode, int buffer, int buffer_pos)
{
    const char modes[3][3] = {"rb", "wb", "ab"};  
    FileStream *stream = malloc(sizeof(FileStream));

    if(stream == NULL)
        err_sys("Allocating memory for FileStream structure");

    stream->fp = fopen(fileName, modes[mode]);
    if(stream->fp == NULL)
        err_sys("Opening file");

    stream->mode = mode;
    stream->buffer = buffer;
    stream->buffer_pos = buffer_pos;

    return stream;
}

/* Function closes stream and free Filestream structure */
void close_file_stream(FileStream *stream)
{
    int return_status;
    if(stream == NULL)
        err_sys("FileStream object does not exist");

    return_status = fclose(stream->fp);
    if(return_status < 0)
        err_sys("Closing file");

    free(stream);
}

/* Function clears output buffer */
void clear_output_buffer(FileStream *output_stream)
{
    int return_status;
    if(output_stream->buffer_pos != 0){
        return_status = fputc(output_stream->buffer, output_stream->fp);
        if(return_status == EOF)
            err_sys("Writing to file");
    }
}

/* Function writes code of length code_length to stream */
void write_code(FileStream *stream, unsigned int code, unsigned int code_length)
{
    int return_status;
    int mask_pos = 0;
    
    while (code_length--)
    {
        /* Check if there is 0 or 1 in code at mask_pos and put result in buffer at buffer_pos */
        if(code & (1 << mask_pos))
            stream->buffer |= 1 << stream->buffer_pos;
        else
            stream->buffer |= 0 << stream->buffer_pos;
        mask_pos++;
        
        /* If buffer is full then output */
        if (stream->buffer_pos++ == 7) 
        {
            return_status = fputc(stream->buffer, stream->fp);
            if(return_status == EOF)
                err_sys("Writing to file");

            stream->buffer = 0;
            stream->buffer_pos = 0;
        }
    }
}

/* Function writes byte to stream */
void write_byte(FileStream *stream, unsigned char byte){
    int return_value;
    return_value = fputc(byte, stream->fp);
    if(return_value == EOF)
        err_sys("Writing to file");
}

/* Function reads code of length code_length from stream */
bool read_code(FileStream *stream, unsigned int *code, unsigned int code_length)
{
    *code = 0;
    for (unsigned int i = 0; i < code_length; i++)
    {
        int bit = next_bit(stream);
        if (bit == EOF)
            return false;
        *code = (*code) | ( bit << i );
    }
    return true;
}

/* Function reads one bit from buf and returns its value */
int next_bit(FileStream *stream){
    int bit;

    /* If max bit is reached */
    if (stream->buffer_pos == 256)
    {
        stream->buffer_pos = 1;
        stream->buffer = fgetc(stream->fp);
        if (stream->buffer == EOF)
            return EOF;
    }

    if(stream->buffer & stream->buffer_pos)
        bit = 1;
    else
        bit = 0; 
    stream->buffer_pos <<= 1;
    
    return bit;
}