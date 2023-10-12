#include "tileblaster.h"
#include <stdio.h>
#include <string.h>

#define ERROR_ARGUMENTS 1
#define ERROR_FILE 2


int main(int argc, char **argv){

    /* Making sure the input filename has been supplied and nothing more */
    if (argc != 2) return ERROR_ARGUMENTS;

    /* Allocating memory to know if there are any problems with the problem format */
    int *error = (int *) malloc(sizeof(int));
    uint score, line, column;
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
    
    Board *board;
    CellList *cluster; 
    
    while (1){
        /* Reseting error */
        *error = 0;

        /* Reading a single problem and creating a board for it */
        board = getBoard(fileIn, error);

        /* Checking if the problem is invalid or not */
        if (*error == 1) {
            /* Writing the problem header and moving on to the next problem after freeing the board */
            fprintf(fileOut, "%i %i %i\n\n", 
                    board->lines, board->columns, board->variant);

            freeBoard(board);
            continue;
        }
        // check for end of file
        if (!board) break;
        applyGravity(board);

        /* Creating a cluster with the tile in the problem */
        int color = board->tiles[line][column-1];

        score = findCluster(board, line, column, color);
        score = score * (score - 1);

        // if it's only one tile revert the -1 tranformation
        if (score == 0) board->tiles[line-1][column-1] = color;

        if (board->variant == 2)
            applyGravity(board);

        /* Writing to the output file */
        writeFile(fileOut, board, score);

        freeBoard(board);
    }
    /* Freeing the variable error and closing the files before closing the programm */
    free(error);
    free(filenameOut);

    fclose(fileIn);
    fclose(fileOut);
    return 0;
}
