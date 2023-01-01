#include <stdlib.h>
#include "file_stream.h"
#include "err_sys.h"

/* Additional functions */
void write_bit(FileStream *stream, int bit);

/* Function initialize and returns FileStream structure */
FileStream* open_file_stream(char *fileName, Mode mode, int char_buffer, int char_buffer_pos)
{
    const char modes[3][3] = {"rb", "wb", "ab"};  
    FileStream *stream = malloc(sizeof(FileStream));

    if(stream == NULL)
        err_sys("Allocating memory for FileStream structure");

    stream->fp = fopen(fileName, modes[mode]);
    if(stream->fp == NULL)
        err_sys("Opening file");

    stream->mode = mode;
    stream->char_buffer = char_buffer;
    stream->char_buffer_pos = char_buffer_pos;

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

/* Function clears char_buffer */
void clear_output_buffer(FileStream *stream)
{
    int return_status;
    if(stream->char_buffer_pos != 0){
        return_status = fputc(stream->char_buffer, stream->fp);
        if(return_status == EOF)
            err_sys("Writing to file");
    }
}

/* Function reads byte from stream and outputs its value */
int read_byte(FileStream *stream)
{
    unsigned char code = 0;
    for (int i = 0; i < 8; i++)
    {
        int bit = read_bit(stream);
        if (bit == EOF)
            return EOF;
        code = code | (bit << i);
    }
    return code;
}

/* Function reads one bit from stream and returns its value */
int read_bit(FileStream *stream)
{
    int bit;
    /* If max bit is reached */
    if (stream->char_buffer_pos == 256)
    {
        stream->char_buffer_pos = 1;
        stream->char_buffer = fgetc(stream->fp);
        if (stream->char_buffer == EOF)
            return EOF;
    }

    if(stream->char_buffer & stream->char_buffer_pos)
        bit = 1;
    else
        bit = 0;

    stream->char_buffer_pos <<= 1;
    return bit;
}

/* Function writes code of length code_length to stream */
void write_code(FileStream *stream, char *code, unsigned int code_length)
{
    int mask_pos = 0;
    while (code_length--){
        write_bit(stream, code[mask_pos]);
        mask_pos++;
    }
}

/* Function writes byte to stream */
void write_byte(FileStream *stream, unsigned char byte)
{
    int bit;
    for (int mask_pos = 0; mask_pos < 8; mask_pos++)
    {
        bit = byte & (1 << mask_pos);
        write_bit(stream, bit);
    }
}

/* Function writes bit to stream */
void write_bit(FileStream *stream, int bit)
{
    int return_status;

    if(bit)
        stream->char_buffer |= 1 << stream->char_buffer_pos;
    else
        stream->char_buffer |= 0 << stream->char_buffer_pos;

    /* If char_buffer is full then output */
    if (stream->char_buffer_pos++ == 7) 
    {
        return_status = fputc(stream->char_buffer, stream->fp);
        if(return_status == EOF)
            err_sys("Writing to file");
        stream->char_buffer = 0;
        stream->char_buffer_pos = 0;
    }
}