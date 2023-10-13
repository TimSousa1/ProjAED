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

typedef struct _moveList{
    int line;
    int column;

    struct _moveList *next;
} MoveList;


Board* getBoard(FILE *file, int *error);
void freeBoard(Board *board);

int findCluster(Board*, int line, int column, int color, uint originalID);
void findAllClusters(Board*);

int findTopSweep(Board*);
int findBottomSweep(Board*);
int findLargest(Board*);

uint removeCluster(Board *board, int id);
void resetClusterSets(Board*);

void applyGravity(Board*);

MoveList *moveListAdd(MoveList *head, uint line, uint column);
void freeMoveList(MoveList *head);

void showBoard(Board*);
void showID(Board*);

char *outputName(char *inputName);
void writeFile(FILE *file, Board *board, MoveList *allMoves, uint score);

#endif
