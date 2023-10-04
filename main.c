#include "common.h"
#include <stdio.h>
#include <string.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2

//TODO: fix read for other variants and matrix sizes
// free board properly

int main(int argc, char **argv){

    /* Making sure the input filename has been supplied and nothing more */
    if (argc != 2) return ERROR_ARGUMENTS;

    /* Allocating memory to know if there are any problems with the problem format */
    short *error = (short *) malloc(sizeof(short));

    /* Creating the name for the output file */
    char *filenameIn = argv[1];
    char *filenameOut;
    char *name = (char *) malloc((strlen(filenameIn) + 1) * sizeof(char));
    CellList *cluster;
    
    name = strcpy(name, filenameIn);
    name = strtok(name, ".");
    filenameOut = strcat(name, ".singlestep");

    /* Opening the input and output files */
    FILE *fileIn = fopen(filenameIn, "r");
    FILE *fileOut = fopen(filenameOut, "w");

    /* Making sure the files were opened correctly */
    if (!fileIn || !fileOut) return ERROR_FILE;
    
    Board *tiles;

    while (1){
        /* Reseting error */
        *error = 0;

        /* Reading a single problem and creating a board for it */
        tiles = readFile(fileIn, error);
        // check for end of file
        if (!tiles) break;
        /* Checking if the problem ws invalid or not */
        if (*error == 1) {
            /* Writing the problem header and moving on to the next problem after freeing the board */
            fprintf(fileOut, "%hi %hi %hi %hi %hi\n\n", 
                    tiles->lines, tiles->columns, tiles->variant, tiles->l, tiles->c);
            freeBoard(tiles);
            continue;
        }
        //showBoard(tiles);
        /* Creating a cluster with the tile in the problem */
        cluster = findTileCluster(tiles, tiles->l, tiles->c);
        /* Writing to the output file*/
        writeFile(fileOut, tiles, cluster);
        /* Freeing the board as it is no longer necessary */
        freeCluster(cluster);
        freeBoard(tiles);
    }
    /* Freeing the variable error and closing the files before closing the programm */
    free(error);
    free(name);
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}
