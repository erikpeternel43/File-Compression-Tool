#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "../lib/decode.h"
#include "../lib/encode.h"

int main(int argc, char * argv[]){
    huff_encode("compress.txt", "compressed");

    return 0;

}