#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

typedef struct _board {

    int lines;
    int columns;

    int variant;

    int c, l;

    int **tiles;
    uint *clusterSets;

} Board;

typedef struct _cellList{
    int line;
    int column;
    int color;

    struct _cellList *next;
} CellList;


Board* getBoard(FILE *file, int *error);

CellList *findCluster(Board*, int line, int column);
void removeCluster(Board *board, CellList *head); 

uint getScore(CellList *head);
void showBoard(Board*);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, uint score);

void freeBoard(Board *board);
void freeCluster(CellList *head);

#endif
