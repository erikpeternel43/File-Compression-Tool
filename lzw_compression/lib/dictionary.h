#ifndef dictionary_h
#define dictionary_h

/* Structure for every entry in dictionary - used in encoding */
typedef struct Dictionary_node Dictionary_node;

struct Dictionary_node
{
    
    unsigned int  code;              /* Current code */
    unsigned char suffix;            /* Added character */
    unsigned int  prefix_code;       /* Prefix code */

    Dictionary_node *left_child;     /* Left child pointer */
    Dictionary_node *right_child;    /* Right child pointer */
    
};

/* Structure of dictionary for decoding */
typedef struct Decode_dict_entry
{

    unsigned int  prefix_code;       /* Code of prefix */
    unsigned char suffix;            /* Added character */

} Decode_dict_entry;


/* Functions */
Dictionary_node * create_node(unsigned int code, unsigned int prefix_code, unsigned char suffix);
Dictionary_node * find_dictionary_node(Dictionary_node * root, unsigned int search_key);
void dictionary_dispose(Dictionary_node * root);
unsigned int make_key(unsigned int prefix_code, unsigned char suffix);

#endif
