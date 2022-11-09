/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_stream.h"
#include "dictionary.h"
#include "lzw.h"

/* Macro for errors */
#define err_sys(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

void lzw_encode(char *input, char *ouput)
{
    unsigned char current_code_length = MIN_CODE_LENGTH;    
    unsigned int next_code = 256; 
    unsigned int search_key;
    int byte;
    unsigned int code;
    Dictionary_node *root;              
    Dictionary_node *node;              

    /* Open streams */
    FileStream *input_stream = open_file_stream(input, FILE_READ, 0, 0);
    FileStream *output_stream = open_file_stream(ouput, FILE_WRITE, 0, 0);

    /* Get first byte from file or return if file is empty */
    if((byte = fgetc(input_stream->fp)) == EOF)
        return;
    code = byte;
    write_code(output_stream, code, current_code_length);

    /* Create root with first 2 characters, return if there is no second byte */
    if((byte = fgetc(input_stream->fp)) != EOF)
    {
        root = create_node(next_code, code, byte);
        code = byte;
        next_code++;
    }
    else
        return;

    /* Get through rest of the file */
    while((byte = fgetc(input_stream->fp)) != EOF)
    {
        search_key = make_key(code, byte);
        node = find_dictionary_node(root, search_key);
        
        if(node->prefix_code == code && node->suffix == byte)
            /* Entry found */
            code = node->code;
        else 
        {
            /* Entry not found.  
            First check if dictionary is full */

            if(next_code < MAX_CODE)
            {
                Dictionary_node * temp = create_node(next_code, code, byte);

                if(make_key(code, byte) < make_key(node->prefix_code, node->suffix)) 
                    node->left_child = temp;
                else 
                    node->right_child = temp;

                next_code++;
            }
            else
                err_sys("Dictionary is full");

            /* Check if code overflows max number that could be written with current_code_lenght bits.
               In case of increase insure decoder will know when to increase - write max number (all 1s).
            */
            while ((code >= (CURRENT_MAX_CODES(current_code_length) - 1)) && (current_code_length < MAX_CODE_LENGTH))
            {
                unsigned int ones = CURRENT_MAX_CODES(current_code_length) - 1;
                write_code(output_stream, ones, current_code_length);
                current_code_length++;
            }

            write_code(output_stream, code, current_code_length);
            code = byte;
        }
    }
    /* Write last found code */
    write_code(output_stream, code, current_code_length);

    dictionary_dispose(root);
    close_file_stream(input_stream);
    close_file_stream(output_stream);
}
