#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

void applyGravity(Board *board);

/******************************************************************************
 * findCluster ()
 *
 * Arguments: Board *board, int line and int column
 * Returns: CellList *
 * Side-Effects: Changes the clusterSets of the first argument (Board *)
 *
 * Description: Groups the tile provided as an argument in a cluster
 *****************************************************************************/

int findCluster(Board *board, int line, int column, int clusterColor){
    if (line > board->lines || line <= 0 ||
            column > board->columns || column <= 0)
        return 0;

    int color;
    color = board->tiles[line - 1][column - 1];
    
    if (color != clusterColor || color == -1) return 0;

    board->tiles[line - 1][column - 1] = -1;
    int score;
    score = 1;
    score += findCluster(board, line + 1, column, clusterColor); // up
    score += findCluster(board, line - 1, column, clusterColor); // down
    score += findCluster(board, line, column - 1, clusterColor); // left
    score += findCluster(board, line, column + 1, clusterColor); //right

    return score;
}


void applyGravity(Board *board) {

    int line, column, counter;
    
    /* Simulating vertical gravity */
    for (column = 0; column < board->columns; column++) {
        /* Setting the counter of empty tiles to 0 */
        counter = 0;
        for (line = 0; line < board->lines; line++) {

            /* Counting the number of empty tiles */
            if (board->tiles[line][column] == -1) {
                counter++;
            /* Making a tile fall */
            } else if (counter) {
                board->tiles[line - counter][column] = board->tiles[line][column];
                board->tiles[line][column] = -1;
            }
        }
    }

    /* Simulating horizontal gravity */
    counter = 0;
    for (column = board->columns - 1; column >= 0; column--) {
        /* Counting the number of empty columns */
        if (board->tiles[0][column] == -1) {
            counter++;
        /* Sliding a column to the right */
        } else if (counter) {
            for (line = 0; line < board->lines; line++) {
                board->tiles[line][column + counter] = board->tiles[line][column];
                board->tiles[line][column] = -1;
            }
        }
    }
    return;
}


void freeBoard(Board *board) {

    int i;
    /* Freeing the memory allocated for the tiles matrix */
    for (i = 0; i < board->lines; i++) {
        free(board->tiles[i]);
    }
    free(board->tiles);
    
    /* Freeing the memory allocated for the board */
    free(board);

    return;
}

void showBoard(Board *board){
    for (int i = board->lines - 1; i >= 0; i--) {
        for (int j = 0; j < board->columns; j++) {
            fprintf(stdout, "%i ", board->tiles[i][j]);
        }
        fprintf(stdout, "\n");
    } 
    fprintf(stdout, "\n");
}
