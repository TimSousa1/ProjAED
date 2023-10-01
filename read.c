#include "read.h"
#include <stdio.h>
#include <stdlib.h>

Board *readFile(FILE *file){

    Board *board = (Board *) malloc (sizeof(Board));

    // getting the header of the tiles file
    if (fscanf(file, "%hi %hi %hi %hi %hi",
        &board->lines, &board->columns, &board->variant, &board->l, &board->c) != 5) 
            return NULL;    

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
    board->clusterSets = (ushort *) malloc (sizeof(ushort) * board->lines * board->columns);
    //printf("creating clusterSet\n");
    for (ushort i = 0; i < board->lines * board->columns; i++){
        board->clusterSets[i] = i;
        //printf("%i ", board->clusterSets[i]);
    }
    //printf("\n");
    return board;
}

void writeFile(FILE *file, Board *board) {

    short score = 0, cluster, i, j;

    if (board->variant == 1) {
        fprintf(file, "%hi %hi %hi %hi %hi\n",
                board->lines, board->columns, board->variant, board->l, board->c);

        cluster = board->clusterSets[board->l * board->columns + board->c];

        for (i = 0; i < board->lines * board->columns; i++) {
            if (board->clusterSets[i] == cluster) score++;
        }

        fprintf(file, "%hi\n", score * (score - 1));

    }

    return;
    
}
