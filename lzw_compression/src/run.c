#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "../lib/lzw.h"


int main(int argc, char * argv[]){
    int c;
    char encode = 1;
    char * input = NULL;
    char * output = NULL;
    
    while ((c = getopt (argc, argv, "cdi:o:h?")) != -1){
        switch (c)
        {
        case 'c':
            encode = 1;
            break;
        case 'd':
            encode = 0;
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        case 'h':
        case '?':
            printf("Usage for encode: /a.out -c -i <input file> -o <output file> \n");
            printf("Usage for decode: /a.out -d -i <input file> -o <output file> \n");
            return 0;
        }
    }

    if(encode)
        lzw_encode(input, output);
    else
        lzw_decode(input, output);

    return 0;

}