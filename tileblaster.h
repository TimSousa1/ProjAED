#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

typedef struct _board {

    int lines;
    int columns;

    int variant;

    int **tiles;

} Board;

typedef struct _moveList{
    int line;
    int column;

    struct _moveList *next;
} MoveList;


Board* getBoard(FILE *file, int *error);

int findCluster(Board*, int line, int column, int color);
void applyGravity(Board*);

void showBoard(Board*);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, MoveList *allMoves, uint score);

void freeBoard(Board *board);

#endif
