#include <stdio.h>
#include <stdlib.h>
#include "file_stream.h"
#include "dictionary.h"
#include "huffman.h"
#include "err_sys.h"

void huff_decode(char *input, char *output)
{
    DictNode *root;
    DictNode *node;
    int byte;
    int bit;

    /* Open streams */
    FileStream *input_stream = open_file_stream(input, FILE_READ, 0, 256);
    FileStream *output_stream = open_file_stream(output, FILE_WRITE, 0, 0);

    /* Create array for all possible chars */
    Symbol **symbols_array = calloc(MAX_VALUE, sizeof(Symbol*));
    if(symbols_array == NULL)
        err_sys("Allocating memory for Symbol array");

    /* Create root */
    root = create_node(true, true, true, NULL, EMPTY_VALUE, 0, MAX_VALUE * 2);  /* Max positions possible (256 * 2) */
    node = root;

    while (1)
    {
        node = root;

        /* Travel to leaf */
        while(!node->is_leaf)
        {
            bit = read_bit(input_stream);
            if(bit)
                node = node->right_child;
            else
                node = node->left_child;
        }

        if(node->is_zero)
        {
            /* Char not yet in dictionary */
            byte = read_byte(input_stream);
            if(byte == EOF)
                break;
            write_byte(output_stream, byte);

            /* Insert new node and symbol */
            DictNode *new_node = dict_insert(byte, &node);
            symbols_array[byte] = malloc(sizeof(Symbol));
            if(symbols_array[byte] == NULL)
                err_sys("Allocating memory for Symbol structure");
            symbols_array[byte]->node = new_node->right_child;
            symbols_array[byte]->symbol = byte;

            validate_and_update(new_node, root);
        }
        else
        {
            /* Char was found in dictionary */
            write_byte(output_stream, node->symbol);
            validate_and_update(node, root);
        }
    }

    /* Write any bytes left in io_buffer to stream */
    clear_output_buffer(output_stream);

    dictionary_dispose(root);
    close_file_stream(input_stream);
    close_file_stream(output_stream);

}
