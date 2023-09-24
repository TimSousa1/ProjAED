#include "read.h"
#include <stdio.h>
#include <stdlib.h>

short findTileCluster(Board *board, short line, short column){
    short score;

    score = board->tilesBoard[line - 1][column - 1];
    return score;
}

void showBoard(Board *board){

    for (short i = board->lines - 1; i >= 0; i--){
        for (short j = 0; j < board->columns; j++){
            printf("%hi ", board->tilesBoard[i][j]);
        }
        printf("\n");
    }
}
