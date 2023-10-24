#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector2 {
    int x;
    int y;
} Vector2;

typedef struct _vectorlist {
    Vector2 tile;
    
    struct _vectorlist *next;
} VectorList;

typedef struct _tileList{
    Vector2 initPos;
    Vector2 finalPos;

    struct _tileList *next;
} TileList;

typedef struct _solution {
    uint score;
    VectorList *moves;
} Solution;

typedef struct _cluster {
    Vector2 tile;
    int id;
    uint numberOfTiles;
    int color;
    VectorList *tiles;
} Cluster;

typedef struct _clusterlist {
    Cluster cluster;
    struct _clusterlist *next;
} ClusterList;

typedef struct _board {

    int lines;
    int columns;

    int variant;

    uint *colors;
    uint numColors;

    Vector2 **tiles;

} Board;

typedef struct _moveList {
    Vector2 tile;
    int color;
    int id;
    int score;

    VectorList *clusters;
    Board *board;

    struct _moveList *next;
} MoveList;

Board *getBoard(FILE*, int *error);
Board *copyBoard(Board*);
void freeBoard(Board *board);

void countColors(Board *board);

int findCluster(Board*, int line, int column, int color, int id, bool toRemove);
VectorList *findAllClusters(Board*);

int idSweep(Board *board, int lastID);
int findTopSweep(Board*);
Vector2 findBottomSweep(Board*, Vector2 play);
int findLargest(Board*);

MoveList *removeCluster(Board *board, Vector2 tile);
void resetClusterSets(Board*);

VectorList *addToVectorList(VectorList *head, Vector2 tile);
void applyGravity(Board*);

uint hopeless(Board *board, int goal, MoveList *head);

TileList *addToTileList(TileList *head, Vector2 initPos, Vector2 finalPos);

Solution solve(Board*);
void freeMoveList(MoveList *head);
void freeVectorList(VectorList *head);

void showBoard(Board*);
void showID(Board*);
void showTileList(TileList *tiles);
void showMoveList(MoveList *lastMove);
void showVectorList(VectorList*);


char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, Solution answer);

#endif
