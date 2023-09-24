#include "read.h"
#include <stdio.h>
#include <stdlib.h>

Board *readFile(char *filename){

    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    Board *board = (Board *) malloc (sizeof(Board));

    fscanf(file, "%hi %hi %hi %hi %hi",
            &board->lines, &board->columns, &board->variant, &board->l, &board->c);

    return board;
}
