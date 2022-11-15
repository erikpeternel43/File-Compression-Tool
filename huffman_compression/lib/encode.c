/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_stream.h"
#include "dictionary.h"
#include "encode.h"

void huff_encode(char *input, char *output){
    DictNode * root;
    DictNode * zero_node;
    int byte;
    int code_lenght;

    /* Open streams */
    FileStream * input_stream = open_file_stream(input, FILE_READ, 0, 0);
    FileStream * output_stream = open_file_stream(output, FILE_WRITE, 0, 0);

    /* Create array for all possible symbols */
    Symbol ** symbols_array = calloc(MAX_VALUE, sizeof(Symbol *));

    /* Create root */
    root = create_node(true, true, true, NULL, EMPTY_VALUE, 0, MAX_VALUE * 2); /* Max positions possible (256 * 2) */
    zero_node = root;

    while ((byte = fgetc(input_stream->fp)) != EOF)
    {
        Symbol * symbol = symbols_array[byte];
        /* Byte in dictionary */
        if(symbol){
            char * code = get_code_from_node(symbol->node, &code_lenght);
            //write_code(output_stream, code, code_lenght);

            validate_and_update(symbol->node, root);
            free(code);
        }
        /* Byte not in dictionary */
        else{
            char * zero_code = get_code_from_node(zero_node, &code_lenght);
            //write_code(output_stream, zero_code, code_lenght);
            //write_byte(output_stream, byte);

            DictNode *new_node = dict_insert(byte, &zero_node);
            symbols_array[byte] = malloc(sizeof(Symbol));
            symbols_array[byte]->node = new_node->right_child;
            symbols_array[byte]->symbol = byte;

            validate_and_update(new_node, root);
            free(zero_code);
        }
    }
    close_file_stream(input_stream);
    close_file_stream(output_stream);

}
