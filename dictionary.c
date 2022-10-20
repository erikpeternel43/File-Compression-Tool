/* Included files */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include "lzw.h"

/* Macro for errors */
#define err_sys(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

/* Function allocates memory and returns pointer to new Dictionary_node structure */
Dictionary_node * create_node(unsigned int code, unsigned int prefix_code, unsigned char suffix) 
{
    Dictionary_node * node = malloc(sizeof(Dictionary_node));
    
    if(node != NULL)
    {
        node->code = code;
        node->prefix_code = prefix_code;
        node->suffix = suffix;
        node->right_child = NULL;
        node->left_child = NULL;
    }
    else
        err_sys("Allocating memory for dictionary node");

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
Dictionary_node * find_dictionary_node(Dictionary_node * root, unsigned int search_key)
{
    unsigned int key = make_key(root->prefix_code, root->suffix);
    /* Node is found */
    if (key == search_key)
        return root;
    else if (search_key < key)
    {
        if (root->left_child != NULL)
            root = find_dictionary_node(root->left_child, search_key);
    }
    else
    {
        if (root->right_child != NULL)
            root = find_dictionary_node(root->right_child, search_key);
    }
    /* Node is not found - return root node */
    return root;
}


void dictionary_dispose(Dictionary_node * root){
    if(root == NULL)
        return;
    if(root->left_child != NULL)
        dictionary_dispose(root->left_child);
    if(root->right_child != NULL)
        dictionary_dispose(root->right_child);
    free(root);
}


