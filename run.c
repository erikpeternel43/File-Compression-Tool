#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lzw.h"

void bad_input(){
    printf("Incorect usage. \n");
    printf("Usage for encode: /a.out -c -i <input file> -o <output file> \n");
    printf("Usage for decode: /a.out -d -i <input file> -o <output file> \n");
    exit(0);
}

int main(int argc, char * argv[]){
    if(argc != 6)
        bad_input();
    if(strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-d") != 0)
        bad_input();
    if(strcmp(argv[2], "-i") != 0 || strcmp(argv[4], "-o"))
        bad_input();
    if(strcmp(argv[1], "-c") == 0)
        lzw_encode(argv[3], argv[5]);
    else
        lzw_decode(argv[3], argv[5]);
    return 0;    
}