#include "read.h"
#include <stdio.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2

int main(int argc, char **argv){

    if (argc != 2) return ERROR_ARGUMENTS;

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) return ERROR_FILE;

    Board *tiles;

    while (1){
        tiles = readFile(file);

        // check for end of file
        if (!tiles) break;

        showBoard(tiles);
        short s = findTileCluster(tiles, tiles->lines, tiles->columns);
        printf("%hi\n", s);
    }
    return 0;
}
