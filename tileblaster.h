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

    uint *colors;
    uint numColors;

    Vector2 **tiles;

} Board;

typedef struct _vectorlist{
    Vector2 tile;
    
    struct _vectorlist *next;
} VectorList;

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

    VectorList *removedTiles;

    TileList *tileHead;
    uint tilesMoved;

    struct _moveList *next;
} MoveList;

typedef struct _solution {
    uint score;
    VectorList *moves;
} Solution;

Board* getBoard(FILE *file, int *error);
void freeBoard(Board *board);

void countTiles(Board *board);

int findCluster(Board*, int line, int column, int color, uint originalID);
void findAllClusters(Board*);

int findTopSweep(Board*);
int findBottomSweep(Board*, Vector2 lastPlay);
int findLargest(Board*);

MoveList *removeCluster(Board *board, int id);
void resetClusterSets(Board*);

VectorList *addToVectorList(VectorList *head, Vector2 tile);
TileList *applyGravity(Board*, uint *tilesMoved);

uint hopeless(Board *board, int goal, MoveList *head);

TileList *addToTileList(TileList *head, Vector2 initPos, Vector2 finalPos);

Solution solveVariant1(Board *board);
Solution solveVariant2or3(Board *board);
void freeMoveList(MoveList *head);
void freeVectorList(VectorList *head);

void showBoard(Board*);
void showID(Board*);
void showTileList(TileList *tiles);
void showMoveList(MoveList *lastMove);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, Solution answer);

#endif
