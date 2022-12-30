#include <stdio.h>
#include <stdlib.h>
#include "file_stream.h"
#include "dictionary.h"
#include "huffman.h"
#include "err_sys.h"


void huff_encode(char *input, char *output)
{
    DictNode *root;
    DictNode *zero_node;
    int byte;
    int code_lenght;
    char *zero_code;
    char *code;

    /* Open streams */
    FileStream *input_stream = open_file_stream(input, FILE_READ, 0, 0);
    FileStream *output_stream = open_file_stream(output, FILE_WRITE, 0, 0);

    /* Create array for all possible chars */
    Symbol **symbols_array = calloc(MAX_VALUE, sizeof(Symbol*));
    if(symbols_array == NULL)
        err_sys("Allocating memory for Symbol array");

    /* Create root */
    root = create_node(true, true, true, NULL, EMPTY_VALUE, 0, MAX_VALUE * 2);   /* Max positions possible (256 * 2) */
    zero_node = root;

    while ((byte = get_char_from_buffer(input_stream)) != EOF)
    {

        Symbol *symbol = symbols_array[byte];
        if(symbol)
        {
            /* Byte in dictionary */
            code = get_code_from_node(symbol->node, &code_lenght);
            write_code(output_stream, code, code_lenght);
            validate_and_update(symbol->node, root);
            free(code);
        }
        else
        {
            /* Byte not in dictionary */
            zero_code = get_code_from_node(zero_node, &code_lenght);
            write_code(output_stream, zero_code, code_lenght);
            write_byte(output_stream, byte);

            /* Insert new node and symbol */
            DictNode *new_node = dict_insert(byte, &zero_node);
            symbols_array[byte] = malloc(sizeof(Symbol));
            if(symbols_array[byte] == NULL)
                err_sys("Allocating memory for Symbol structure");
            symbols_array[byte]->node = new_node->right_child;
            symbols_array[byte]->symbol = byte;

            validate_and_update(new_node, root);
            free(zero_code);
        }
    }

    /* Adding last zero code to mark end of file */
    zero_code = get_code_from_node(zero_node, &code_lenght);
    write_code(output_stream, zero_code, code_lenght);

    /* Write any bits left in output buffer to stream */
    clear_output_buffer(output_stream);

    free(zero_code);
    dictionary_dispose(root);
    close_file_stream(input_stream);
    close_file_stream(output_stream);

}
