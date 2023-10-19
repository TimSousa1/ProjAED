#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

typedef struct Vector2 {
    int x;
    int y;
} Vector2;

typedef struct _board {

    int lines;
    int columns;

    int variant;
    int score;
    
    Vector2 **tiles;

} Board;

typedef struct _tileList{
    Vector2 initPos;
    Vector2 finalPos;

    struct _tileList *next;
} TileList;

typedef struct _moveList {
    Vector2 tile;
    int color;
    int id;
    int score;

    TileList *tileHead;

    struct _moveList *previous;
    struct _moveList *next;
} MoveList;

Board* getBoard(FILE *file, int *error);
void freeBoard(Board *board);

int findCluster(Board*, int line, int column, int color, uint originalID);
void findAllClusters(Board*);

int findTopSweep(Board*);
int findBottomSweep(Board*);
int findLargest(Board*);

MoveList *removeCluster(Board *board, int id);
void resetClusterSets(Board*);

TileList *applyGravity(Board*);

TileList *addToTileList(TileList *head, Vector2 initPos, Vector2 finalPos);

MoveList *solveVariant1(Board *board);
MoveList *solveVariant2or3(Board *board);
void freeMoveList(MoveList *head);

void showBoard(Board*);
void showID(Board*);
void showTileList(TileList *tiles);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, MoveList *allMoves, uint score);

#endif
