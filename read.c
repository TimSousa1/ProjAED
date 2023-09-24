#include "read.h"
#include <stdio.h>
#include <stdlib.h>

Board *readFile(char *filename){

    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    Board *board = (Board *) malloc (sizeof(Board));

    // getting the header of the tiles file
    fscanf(file, "%hi %hi %hi %hi %hi",
            &board->lines, &board->columns, &board->variant, &board->l, &board->c);

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

    return board;
}

void showBoard(Board *board){

    for (short i = board->lines - 1; i >= 0; i--){
        for (short j = 0; j < board->columns; j++){
            printf("%hi ", board->tilesBoard[i][j]);
        }
        printf("\n");
    }
}
