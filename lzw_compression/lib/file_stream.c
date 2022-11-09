/* Included files */
#include "file_stream.h"
#include <stdlib.h>

/* Macro for errors */
#define err_sys(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

/* Additional functions */
static int next_bit(FileStream *stream);

/* Function initialize and returns FileStream structure */
FileStream *open_file_stream(char *fileName, Mode mode, int buf, int bufpos)
{
    const char modes[3][3] = {"rb", "wb", "ab"};  
    FileStream *stream = malloc(sizeof(FileStream));

    if(stream == NULL)
        err_sys("Allocating memory");

    stream->fp = fopen(fileName, modes[mode]);
    if(stream->fp == NULL)
        err_sys("Opening file");

    stream->mode = mode;
    stream->buf = buf;
    stream->bufpos = bufpos;

    return stream;
}

/* Function closes stream and free Filestream structure */
void close_file_stream(FileStream *stream)
{
    int return_status;

    if(stream == NULL)
        err_sys("Accessing memory");

    /* If theres if bits left in bufpos after writing -> output another byte */
    if(stream->mode == FILE_WRITE && stream->bufpos != 0)
        fputc(stream->buf, stream->fp);

    return_status = fclose(stream->fp);
    if(return_status < 0)
        err_sys("Closing file");

    free(stream);
}

/* Function writes code of length code_length to stream */
void write_code(FileStream *stream, unsigned int code, unsigned int code_length)
{
    int return_status;

    int mask_pos = 0;
    while (code_length--)
    {
        /* Check if there is 0 or 1 in code at mask_pos and put result in buf at bufpos */
        if(code & (1 << mask_pos))
            stream->buf |= 1 << stream->bufpos;
        else
            stream->buf |= 0 << stream->bufpos;
        mask_pos++;
        /* If buffer is full then output */
        if (stream->bufpos++ == 7) 
        {
            return_status = fputc(stream->buf, stream->fp);
            if(return_status == EOF)
                err_sys("Writing to file");

            stream->buf = 0;
            stream->bufpos = 0;
        }
    }
}

/* Function reads code of length - code_length from stream */
int read_code(FileStream *stream, unsigned int code_length)
{
    unsigned int code = 0;
    for (unsigned int i = 0; i < code_length; i++)
    {
        int bit = next_bit(stream);
        if (bit == EOF)
            return EOF;
        code = code | ( bit << i );
    }
    return code;
}

/* Function reads one bit from buf and returns its value */
int next_bit(FileStream *stream){
    int bit;
    /* If max bit is reached */
    if (stream->bufpos == 256)
    {
        stream->bufpos = 1;
        stream->buf = fgetc(stream->fp);
        if (stream->buf == EOF)
            return EOF;
    }
    if(stream->buf & stream->bufpos)
        bit = 1;
    else
        bit = 0;
    stream->bufpos <<= 1;
    return bit;
}