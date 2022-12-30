#include <stdlib.h>
#include "file_stream.h"
#include "err_sys.h"

/* Additional functions */
void write_bit(FileStream *stream, int bit);

/* Function initialize and returns FileStream structure */
FileStream* open_file_stream(char *fileName, Mode mode, int curr_char, int curr_char_pos)
{
    const char modes[3][3] = {"rb", "wb", "ab"};  
    FileStream *stream = malloc(sizeof(FileStream));

    if(stream == NULL)
        err_sys("Allocating memory for FileStream structure");

    stream->fp = fopen(fileName, modes[mode]);
    if(stream->fp == NULL)
        err_sys("Opening file");

    stream->mode = mode;
    stream->curr_char = curr_char;
    stream->curr_char_pos = curr_char_pos;
    stream->buffer = malloc(512 * sizeof(unsigned char));
    stream->buffer_pos = 0;
    stream->buffer_strlen = 0;
    
    if(stream->buffer == NULL)
        err_sys("Allocating memory for buffer");

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
    
    free(stream->buffer);
    free(stream);
}

/* Function clears output buffer */
void clear_output_buffer(FileStream *stream)
{
    if(stream->curr_char_pos != 0)
        put_char_to_buffer(stream, stream->curr_char);
    if(stream->buffer_pos != 0){
        stream->buffer_strlen = fwrite(stream->buffer, sizeof(unsigned char), stream->buffer_pos, stream->fp);
        if(stream->buffer_strlen != stream->buffer_pos)
            err_sys("Writing to file");
    }
}

/* Function reads byte from stream */
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

/* Function reads one bit from current char and returns its value */
int read_bit(FileStream *stream)
{
    int bit;
    /* If max bit is reached */
    if (stream->curr_char_pos == 256)
    {
        stream->curr_char_pos = 1;
        stream->curr_char = get_char_from_buffer(stream);
        if (stream->curr_char == EOF)
            return EOF;
    }

    if(stream->curr_char & stream->curr_char_pos)
        bit = 1;
    else
        bit = 0;

    stream->curr_char_pos <<= 1;
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
    if(bit)
        stream->curr_char |= 1 << stream->curr_char_pos;
    else
        stream->curr_char |= 0 << stream->curr_char_pos;

    /* If buffer is full then output */
    if (stream->curr_char_pos++ == 7) 
    {
        put_char_to_buffer(stream, stream->curr_char);
        stream->curr_char = 0;
        stream->curr_char_pos = 0;
    }
}

int get_char_from_buffer(FileStream *stream)
{
    if(stream->buffer_pos == stream->buffer_strlen){
        stream->buffer_strlen = fread(stream->buffer, sizeof(unsigned char), 512, stream->fp);
        if(ferror(stream->fp))
            err_sys("Reading from file");
        if(feof(stream->fp) && stream->buffer_strlen == 0)
            return EOF;
        stream->buffer_pos = 0;
    }
    unsigned char curr_char = stream->buffer[stream->buffer_pos];
    stream->buffer_pos++;
    return curr_char;
}

void put_char_to_buffer(FileStream *stream, unsigned char character)
{
    if(stream->buffer_pos == 512){
        stream->buffer_strlen = fwrite(stream->buffer, sizeof(unsigned char), 512, stream->fp);
        if(stream->buffer_strlen != 512)
            err_sys("Writing to file");
        stream->buffer_pos = 0;
    }
    stream->buffer[stream->buffer_pos] = character;
    stream->buffer_pos++;
}

