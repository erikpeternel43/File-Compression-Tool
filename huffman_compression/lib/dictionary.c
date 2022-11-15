/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include "encode.h"

/* Recursive function to reverse array */
void reverse_code(char * code, int left_index, int right_index){
    int temp;
    if(left_index <= right_index){
     temp = code[left_index];
     code[left_index] = code[right_index];
     code[right_index] = temp;
     reverse_code(code, left_index+1, right_index-1);
    }
}

/* Function allocates memory and returns pointer to new DictNode structure */
DictNode * create_node(bool is_zero, bool is_root, bool is_leaf, DictNode * parent, unsigned char symbol, int value, int position){
    DictNode * new_node = malloc(sizeof(DictNode));

    new_node->is_zero = is_zero;
    new_node->is_root = is_root;
    new_node->is_leaf = is_leaf;

    new_node->parent = parent;
    new_node->left_child = NULL;
    new_node->right_child = NULL;

    new_node->symbol = symbol;
    new_node->value = value;
    new_node->position = position;

    return new_node;
}

char * get_code_from_node(DictNode * node, int * code_lenght){
    DictNode * temp = node;
    char * code = malloc(MAX_VALUE * 2 * sizeof(char)); 
    int length = 0;

    /* Traverse till root */
    while(!temp->is_root){
        DictNode * parent = temp->parent;
        if(parent->left_child == temp)
            code[length] = 0;
        else
            code[length] = 1;
        length++;
        temp = temp->parent;
    }

    /* Code is written in reverse order so we flip it */
    reverse_code(code, 0, length-1);
    *code_lenght = length;
    return code;
}


DictNode * dict_insert(int byte, DictNode ** zero_node){
    DictNode * right_child = create_node(false, false, true, (*zero_node), (unsigned char) byte, 1, (*zero_node)->position - 1);
    DictNode * left_child = create_node(true, false, true, (*zero_node), EMPTY_VALUE, 0, (*zero_node)->position - 2);

    (*zero_node)->is_zero = false;
    (*zero_node)->is_leaf = false;
    (*zero_node)->left_child = left_child;
    (*zero_node)->right_child = right_child;

    DictNode * temp = *zero_node;
    *zero_node = left_child;

    return temp;
}

DictNode * replacement_node(DictNode * node, DictNode * root){
    DictNode *result = node;
    
    if (root->value > result->value && !root->is_leaf) {
        DictNode *greatestLeft = replacement_node(result, root->left_child);
        if (greatestLeft) result = greatestLeft;
        
        DictNode *greatestRight = replacement_node(result, root->right_child);
        if (greatestRight) result = greatestRight;
    } else if (root->value == result->value && root->position > result->position) {
        result = root;
    }
    
    return (result != node) ? result : NULL;
}

void swap_nodes(DictNode * first, DictNode * second){
    bool left_side_f = false;
    bool left_side_s = false;
    if(first->parent->left_child == first)
        left_side_f = true;
    if(second->parent->left_child == second)
        left_side_s = true;

    if(left_side_f)
        first->parent->left_child = second;
    else
        first->parent->right_child = second;

    if(left_side_s)
        second->parent->left_child = first;
    else
        second->parent->right_child = first;

    int temp_pos = first->position;
    first->position = second->position;
    second->position = temp_pos;

    DictNode * temp = first->parent;
    first->parent = second->parent;
    second->parent = temp;

}

void validate_and_update(DictNode * node, DictNode * root){
    while(!node->is_root){
        DictNode * replacement = replacement_node(node, root);
        if(replacement && replacement != node->parent)
            swap_nodes(node, replacement);
        node->value++;
        node = node->parent;
    }
    node->value++;
}

