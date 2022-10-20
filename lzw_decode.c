/* Included files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_stream.h"
#include "dictionary.h"
#include "lzw.h"

/* Macro for errors */
#define err_sys(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

/* Recursive function to find all characters from specific code and write them to end file */
unsigned char decode_out(unsigned int code, FileStream * output_stream, Decode_dict_entry * dictionary)
{
    unsigned char suffix;
    unsigned char first_c;

    if(code > 255)
    {
        first_c = decode_out(dictionary[code].prefix_code, output_stream, dictionary);
        suffix = dictionary[code].suffix;
    }
    else
    {
        first_c = code;
        suffix = code;
    }
    fputc(suffix, output_stream->fp);

    return first_c;
}

void lzw_decode(char *input, char *output)
{
    unsigned char current_code_length = MIN_CODE_LENGTH;    
    unsigned int next_code = 256; 
    unsigned int dict_size = 512;
    unsigned int code;
    unsigned int old_code;
    unsigned char first_c;

    Decode_dict_entry *dictionary = malloc(dict_size * sizeof(Decode_dict_entry));

    /* Opening streams */
    FileStream *input_stream = open_file_stream(input, FILE_READ, 0, 256);
    FileStream *output_stream = open_file_stream(output, FILE_WRITE, 0, 0);

    /* Get first code from file or return if file is empty */
    if((code = read_code(input_stream, current_code_length)) == EOF)
        return;

    fputc(code, output_stream->fp);
    old_code = code;

    /* Decode rest of the file */
    while((code = read_code(input_stream, current_code_length)) != EOF)
    {
        if(code == CURRENT_MAX_CODES(current_code_length) - 1) 
        {
            current_code_length++;
            continue;
        }
        if(code < next_code)
            /* Code in dictionary */
            first_c = decode_out(code, output_stream, dictionary);
        else
        {
            /* Code not in dictionary */
            unsigned char tmp = first_c;
            first_c = decode_out(old_code, output_stream, dictionary);
            fputc(tmp, output_stream->fp);
        }
        /* Check if dictionary resize is needed */
        if(next_code >= dict_size)
        {
            dict_size *= 2;
            Decode_dict_entry *tmp = realloc(dictionary, dict_size * sizeof(Decode_dict_entry));

            if(tmp != NULL)
                dictionary = tmp;
            else
                err_sys("Realloc memory");
        }
        
        /* Make entry in dictionary */        
        dictionary[next_code].suffix = first_c;
        dictionary[next_code].prefix_code = old_code;    

        next_code++;
        old_code = code;
    }
}
