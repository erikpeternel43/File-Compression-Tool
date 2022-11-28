#include <stdbool.h>

#ifndef DICTIONARY_H
#define DICTIONARY_H

/* Structure for every entry in dictionary */
typedef struct DictNode DictNode;

struct DictNode
{
    bool is_zero;                   
    bool is_root;
    bool is_leaf;
    
    struct DictNode *parent;
    struct DictNode *left_child;
    struct DictNode *right_child;
    
    unsigned char symbol;
    int value;                         
    int position;                      
};

/* Functions */
DictNode* create_node(bool is_zero, bool is_root, bool is_leaf, DictNode *parent, unsigned char symbol, int value, int position);
DictNode* dict_insert(int byte, DictNode **zero_node);
char* get_code_from_node(DictNode *node, int *code_lenght);
void validate_and_update(DictNode *node, DictNode *root);
void dictionary_dispose(DictNode *root);

#endif