#include "common.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * readFile ()
 *
 * Arguments: FILE *file and int *error
 * Returns: Board *board
 * Side-Effects: changes the value of the second argument (error) if the problem is invalid
 *
 * Description: creates a Board and sets it up with the current problem information
 *****************************************************************************/

Board *readFile(FILE *file, int *error){

    Board *board = (Board *) malloc (sizeof(Board));

    // getting the header of the problem
    if (fscanf(file, "%i %i %i %i %i",
        &board->lines, &board->columns, &board->variant, &board->l, &board->c) != 5) {
            free(board);
            return NULL;    
    } 
    /* Checking if the problem is invalid or not */
    if (board->variant < 1 || board->variant > 2 || board->l < 1 ||
        board->c < 1 || board->l > board->lines || board->c > board->columns) {
        *error = 1;
        return board;
    }
    // initializing the board matrix
    board->tilesBoard = (int **) malloc (board->lines * sizeof(int *));

    for (uint k = 0; k < board->lines; k++) {
        board->tilesBoard[k] = (int *) malloc (board->columns * sizeof(int));
    }

    // getting every element off of the file
    for (int i = board->lines - 1; i >= 0; i--){
        for (int j = 0; j < board->columns; j++){
            fscanf(file, "%i", &board->tilesBoard[i][j]);
        }
    }
    /* Initializing the clusterSets array*/
    board->clusterSets = (uint *) malloc (sizeof(uint) * board->lines * board->columns);
    for (uint i = 0; i < board->lines * board->columns; i++){
        board->clusterSets[i] = i;
    }
    
    /* Returns the board created */
    return board;
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
    fprintf(file, "%i %i %i %i %i\n",
                board->lines, board->columns, board->variant, board->l, board->c);

    /* Checking the problem variant to see what to write, if it's 1 it writes the score, otherwise 
    * writes the matrix*/ 
    if (board->variant == 1) {

        fprintf(file, "%u\n", score);

    } else if (board->variant == 2) {

        for (i = board->lines - 1; i >= 0; i--) {
            for (j = 0; j < board->columns; j++) {
                fprintf(file, "%i ", board->tilesBoard[i][j]);
            }
            fprintf(file, "\n");
        } 

    }

    fprintf(file, "\n");

    return;
    
}
