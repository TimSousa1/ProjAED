#include "read.h"
#include <stdio.h>
#include <string.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2

//TODO: fix read for other variants and matrix sizes
// free board properly

int main(int argc, char **argv){

    if (argc != 2) return ERROR_ARGUMENTS;

    char *filenameIn = argv[1];
    char *filenameOut;
    char *name = (char *) malloc((strlen(filenameIn) + 1) * sizeof(char));
    
    name = strcpy(name, filenameIn);
    name = strtok(name, ".");
    filenameOut = strcat(name, ".singlestep");

    FILE *fileIn = fopen(filenameIn, "r");
    FILE *fileOut = fopen(filenameOut, "w");
    if (!fileIn || !fileOut) return ERROR_FILE;
    Board *tiles;

    while (1){
        tiles = readFile(fileIn);
        // check for end of file
        if (!tiles) break;

        //showBoard(tiles);
        findTileCluster(tiles, tiles->l, tiles->c);
        writeFile(fileOut, tiles);
        free(tiles);
    }
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}
