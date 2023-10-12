#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * readFile ()
 *
 * Arguments: FILE *file and int *error
 * Returns: Board *board
 * Side-Effects: changes the value of the second argument (error) if the problem is invalid
 *
 * Description: creates a Board and sets it up with the current problem information
 *****************************************************************************/

Board *getBoard(FILE *file, int *error){

    Board *board = (Board *) malloc (sizeof(Board));

    // getting the header of the problem
    if (fscanf(file, "%i %i %i",
        &board->lines, &board->columns, &board->variant) != 3) {
            free(board);
            return NULL;    
    } 
    /* Checking if the problem is invalid or not */
    
    // initializing the board matrix
    board->tiles = (int **) malloc (board->lines * sizeof(int *));

    for (uint k = 0; k < board->lines; k++) {
        board->tiles[k] = (int *) malloc (board->columns * sizeof(int));
    }

    // getting every element off of the file
    for (int i = board->lines - 1; i >= 0; i--){
        for (int j = 0; j < board->columns; j++){
            fscanf(file, "%i", &board->tiles[i][j]);
        }
    }
    /* Checking if the problem is invalid or not */
    if (board->variant != -1 && board->variant != -3 && board->variant < 0) {
        *error = 1;
    }
    /* Returns the board created */
    return board;
}

char *outputName(char *inputName) {

    char *name;
    char *extension;
    uint len;

    len = strlen(inputName);

    name = (char *) malloc((len + 2) * sizeof(char));
    strcpy(name, inputName);

    extension = name + len - 10;
    memcpy(extension, ".tileblasts", 11);

    return name;
}

/******************************************************************************
 * writeFile ()
 *
 * Arguments: FILE *file, Board *board and int score
 * Returns: nothing
 * Side-Effects: changes the second argument (Board *) if it's the second variant
 *
 * Description: writes to the output file the answer to the problem according to its variant
 *****************************************************************************/

void writeFile(FILE *file, Board *board, uint score) {

    int i, j;

    /* Writing the problem header */
    fprintf(file, "%i %i %i\n",
                board->lines, board->columns, board->variant);

    /* Checking the problem variant to see what to write, if it's 1 it writes the score, otherwise 
    * writes the matrix*/ 
    if (board->variant == 1)
        fprintf(file, "%u\n", score);

    else if (board->variant == 2) {
        for (i = board->lines - 1; i >= 0; i--) {
            for (j = 0; j < board->columns; j++) {
                fprintf(file, "%i ", board->tiles[i][j]);
            }
            fprintf(file, "\n");
        } 
    }
    fprintf(file, "\n");

    return;
}
