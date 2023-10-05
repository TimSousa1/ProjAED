#include "common.h"
#include <stdio.h>
#include <string.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2


int main(int argc, char **argv){

    /* Making sure the input filename has been supplied and nothing more */
    if (argc != 2) return ERROR_ARGUMENTS;

    /* Allocating memory to know if there are any problems with the problem format */
    int *error = (int *) malloc(sizeof(int));
    uint score;
    /* Creating the name for the output file */
    char *filenameIn = argv[1];
    char *filenameOut;
    
    /* Get the output file name */
    filenameOut = outputName(filenameIn); 
    if (!filenameOut) return ERROR_FILE;

    /* Opening the input and output files */
    FILE *fileIn = fopen(filenameIn, "r");
    FILE *fileOut = fopen(filenameOut, "w");

    /* Making sure the files were opened correctly */
    if (!fileIn || !fileOut) return ERROR_FILE;
    
    Board *tiles;
    CellList *cluster; 
    
    while (1){
        /* Reseting error */
        *error = 0;

        /* Reading a single problem and creating a board for it */
        tiles = readFile(fileIn, error);
        /* Checking if the problem is invalid or not */
        if (*error == 1) {
            /* Writing the problem header and moving on to the next problem after freeing the board */
            fprintf(fileOut, "%i %i %i %i %i\n\n", 
                    tiles->lines, tiles->columns, tiles->variant, tiles->l, tiles->c);
            freeBoard(tiles);
            continue;
        }
        // check for end of file
        if (!tiles) break;

        /* Creating a cluster with the tile in the problem */
        cluster = findTileCluster(tiles, tiles->l, tiles->c);
        /* Getting either the score of the cluster or removing depending on what the problem wants */
        if (tiles->variant == 1) score = getScore(cluster);
        else if (tiles->variant == 2) removeCluster(tiles, cluster); 
        /* Writing to the output file */
        writeFile(fileOut, tiles, score);
        /* Freeing the board and the cluster as they are no longer necessary */
        freeCluster(cluster);
        freeBoard(tiles);
    }
    /* Freeing the variable error and closing the files before closing the programm */
    free(error);
    free(filenameOut);
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}
