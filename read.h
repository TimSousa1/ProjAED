#ifndef _TILES
#define _TILES

typedef struct _board {

    short lines;
    short columns;

    short variant;

    short c, l;

    short **tilesBoard;

} Board;

Board* readFile(char *filename);
void showBoard(Board*);

#endif
