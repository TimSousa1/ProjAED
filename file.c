#include "common.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * readFile ()
 *
 * Arguments: FILE * and short *
 * Returns: Board *
 * Side-Effects: changes the value of the second argument (error) if the problem is invalid
 *
 * Description: creates a Board and sets it up with the problem information
 *****************************************************************************/

Board *readFile(FILE *file, short *error){

    Board *board = (Board *) malloc (sizeof(Board));

    // getting the header of the tiles file
    if (fscanf(file, "%hi %hi %hi %hi %hi",
        &board->lines, &board->columns, &board->variant, &board->l, &board->c) != 5) {
            free(board);
            return NULL;    
    } 
    // initializing the board matrix
    board->tilesBoard = (short **) malloc (board->lines * sizeof(short *));

    for (ushort k = 0; k < board->lines; k++) {
        board->tilesBoard[k] = (short *) malloc (board->columns * sizeof(short));
    }

    // getting every element off of the file
    for (short i = board->lines - 1; i >= 0; i--){
        for (short j = 0; j < board->columns; j++){
            fscanf(file, "%hi", &board->tilesBoard[i][j]);
        }
    }
    board->clusterSets = (uint *) malloc (sizeof(uint) * board->lines * board->columns);
    for (uint i = 0; i < board->lines * board->columns; i++){
        board->clusterSets[i] = i;
        //printf("%i ", board->clusterSets[i]);
    }
    
    if (board->variant < 1 || board->variant > 2 || board->l < 1 || board->c < 1) *error = 1; 

    return board;
}

/******************************************************************************
 * writeFile ()
 *
 * Arguments: FILE * and Board *
 * Returns: nothing
 * Side-Effects: changes the second argument (Board *) if it's the second variant
 *
 * Description: writes to the output file the answer to the problem
 *****************************************************************************/

void writeFile(FILE *file, Board *board, CellList *head) {

    short i, j;

    fprintf(file, "%hi %hi %hi %hi %hi\n",
                board->lines, board->columns, board->variant, board->l, board->c);

    if (board->variant == 1) {

        fprintf(file, "%hi\n", getScore(board, head));

    } else if (board->variant == 2) {

        removeCluster(board, head);

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
