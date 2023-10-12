#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

typedef struct _board {

    int lines;
    int columns;

    int variant;

    int **tiles;
    uint *clusterSets;

} Board;

typedef struct _moveList{
    int line;
    int column;

    struct _moveList *next;
} MoveList;


Board* getBoard(FILE *file, int *error);

int findCluster(Board*, int line, int column, int color, uint originalID);
int findAllClusters(Board*);
void applyGravity(Board*);

void showBoard(Board*);
void showID(Board*);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, MoveList *allMoves, uint score);

void freeBoard(Board *board);

#endif
