#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

typedef struct _board {

    short lines;
    short columns;

    short variant;

    short c, l;

    short **tilesBoard;
    uint *clusterSets;

} Board;

typedef struct _cellList{
    int line;
    int column;
    int color;

    struct _cellList *next;
} CellList;

Board* readFile(FILE *file, short *error);
CellList *findTileCluster(Board*, short line, short column);
void writeFile(FILE *file, Board *board, CellList *head);
void freeBoard(Board *board);
void freeCluster(CellList *head);
void removeCluster(Board *board, CellList *head); 
ushort getScore(Board *board, CellList *head);

#endif
