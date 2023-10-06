#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

typedef struct _board {

    int lines;
    int columns;

    int variant;

    int l, c;

    int **tiles;

} Board;

typedef struct _cellList{
    int line;
    int column;
    int color;

    struct _cellList *next;
} CellList;


Board* getBoard(FILE *file, int *error);

int findCluster(Board*, int line, int column, int color);
void removeCluster(Board *board, CellList *head); 
void applyGravity(Board*);

uint getScore(CellList *head);
void showBoard(Board*);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, uint score);

void freeBoard(Board *board);
void freeCluster(CellList *head);

#endif
