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
short findTileCluster(Board*, short line, short column);
void showBoard(Board*);

#endif
