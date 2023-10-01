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
    ushort *clusterSets;

} Board;

Board* readFile(FILE *file);
void findTileCluster(Board*, short line, short column);
void writeFile(FILE *file, Board *board);
void removeCluster(Board *board); 
void showBoard(Board*);

#endif
