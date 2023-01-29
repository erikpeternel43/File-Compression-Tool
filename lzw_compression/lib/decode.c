#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_stream.h"
#include "dictionary.h"
#include "lzw.h"
#include "err_sys.h"

/* Additional functions */
unsigned char decode_out(unsigned int code, FileStream *output_stream, DecodeDictNode *dictionary);

void lzw_decode(char *input, char *output)
{
    unsigned char current_code_length = MIN_CODE_LENGTH;    
    unsigned int next_code = 256; 
    unsigned int dict_size = 512;
    unsigned char first_c;
    unsigned int code;
    unsigned int old_code;

    DecodeDictNode *dictionary = malloc(dict_size * sizeof(DecodeDictNode));
    if(dictionary == NULL)
        err_sys("Allocating memory for DecodeDictNode structures")

    /* Open streams */
    FileStream *input_stream = open_file_stream(input, FILE_READ, 0, 256);
    FileStream *output_stream = open_file_stream(output, FILE_WRITE, 0, 0);

    /* Get first code from file or return if file is empty */
    if(!read_code(input_stream, &code, current_code_length))
        return;

    put_char(output_stream, code);
    old_code = code;
    first_c = code;

    /* Decode rest of the file */
    while(read_code(input_stream, &code, current_code_length))
    {
        /* Check for code_length increase */
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
            //unsigned char tmp = first_c;
            first_c = decode_out(old_code, output_stream, dictionary);
            put_char(output_stream, first_c);
        }
        
        /* Check if dictionary resize is needed */
        if(next_code >= dict_size)
        {
            dict_size *= 2;
            DecodeDictNode *tmp = realloc(dictionary, dict_size * sizeof(DecodeDictNode));
            if(tmp != NULL)
                dictionary = tmp;
            else
                err_sys("Reallocating memory for DecodeDictNode structures");
        }
        
        /* Make entry in dictionary */        
        dictionary[next_code].suffix = first_c;
        dictionary[next_code].prefix_code = old_code;    

        next_code++;
        old_code = code;
    }
    
    /* Write any bytes left in io_buffer to stream */
    clear_output_buffer(output_stream);
    
    free(dictionary);
    close_file_stream(input_stream);
    close_file_stream(output_stream);
}


/* Recursive function to find all characters from specific code and write them to end file */
unsigned char decode_out(unsigned int code, FileStream *output_stream, DecodeDictNode *dictionary)
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
    put_char(output_stream, suffix);

    return first_c;
}
