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
    board->clusterSets = (uint*) malloc (board->lines * board->columns * sizeof(uint));

    for (uint k = 0; k < board->lines; k++) {
        board->tiles[k] = (int *) malloc (board->columns * sizeof(int));
    }

    // getting every element off of the file
    for (int i = board->lines - 1, id = 0; i >= 0; i--){
        for (int j = 0; j < board->columns; j++){
            fscanf(file, "%i", &board->tiles[i][j]);
            board->clusterSets[id] = id;
            id++;
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
    memcpy(extension, ".toleblists", 11);

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

void writeFile(FILE *file, Board *board, MoveList *allMoves, uint score) {

    uint moves;
    MoveList *move;

    /* Writing the problem header */
    fprintf(file, "%i %i %i\n",
                board->lines, board->columns, board->variant);
    for (moves = 0, move = allMoves; move; move = move->next, moves++);
    fprintf(file, "%i %i\n", moves, score);
    for (move = allMoves; move; move = move->next) {
        fprintf(file, "%i %i\n", move->line, move->column);
    }
    fprintf(file, "\n");

    return;
}
