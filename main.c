#include "read.h"
#include <stdio.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2

int main(int argc, char **argv){

    if (argc != 2) return ERROR_ARGUMENTS;

    char *filename = argv[1];

    Board *tiles;
    tiles = readFile(filename);

    if (!tiles) return ERROR_FILE;

    return 0;
}
