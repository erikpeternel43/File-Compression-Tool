#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include "lzw.h"
#include "err_sys.h"

/* Creates and returns new DictNode structure */
DictNode * create_node(unsigned int code, unsigned int prefix_code, unsigned char suffix) 
{
    DictNode * node = malloc(sizeof(DictNode));
    if(node != NULL)
    {
        node->code = code;
        node->prefix_code = prefix_code;
        node->suffix = suffix;
        node->right_child = NULL;
        node->left_child = NULL;
    }
    else
        err_sys("Allocating memory for DictNode structure");

    return node;
}

/* 
   Function creates key based on given prefix_code and suffix value.
   Key is used for searching and inserting new values into dictionary.
*/
unsigned int make_key(unsigned int prefix_code, unsigned char suffix) 
{
    unsigned int key = 0;

    key = suffix & 0xF0;
    key <<= MAX_CODE_LENGTH;
    key |= (prefix_code << 4);
    key |= (suffix & 0x0F);

    return key;
}

/* Recursive function for finding specific node with key search_key */
DictNode * find_DictNode(DictNode * root, unsigned int search_key)
{
    unsigned int key = make_key(root->prefix_code, root->suffix);
    if (key == search_key)
        return root;
    else if (search_key < key)
    {
        if (root->left_child != NULL)
            root = find_DictNode(root->left_child, search_key);
    }
    else
    {
        if (root->right_child != NULL)
            root = find_DictNode(root->right_child, search_key);
    }
    return root;
}

void dictionary_dispose(DictNode * root)
{
    if(root == NULL)
        return;
    if(root->left_child != NULL)
        dictionary_dispose(root->left_child);
    if(root->right_child != NULL)
        dictionary_dispose(root->right_child);
    free(root);
}


