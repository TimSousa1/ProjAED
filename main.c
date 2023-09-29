#include "read.h"
#include <stdio.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2

//TODO: fix read for other variants and matrix sizes
// free board properly

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
        short s = findTileCluster(tiles, tiles->l, tiles->c);
        printf("%hi\n", s);
        free(tiles);
    }
    return 0;
}
