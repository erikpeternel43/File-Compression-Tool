#ifndef dictionary_h
#define dictionary_h

/* Structure for every entry in dictionary - used in encoding */
typedef struct DictNode DictNode;

struct DictNode
{
    unsigned int  code;             
    unsigned char suffix;           
    unsigned int  prefix_code;      

    DictNode *left_child;          
    DictNode *right_child;          
};

/* Structure of dictionary for decoding */
typedef struct DecodeDictNode
{
    unsigned int  prefix_code;      
    unsigned char suffix;          
} DecodeDictNode;


/* Functions */
DictNode * create_node(unsigned int code, unsigned int prefix_code, unsigned char suffix);
DictNode * find_DictNode(DictNode * root, unsigned int search_key);
void dictionary_dispose(DictNode * root);
unsigned int make_key(unsigned int prefix_code, unsigned char suffix);

#endif
