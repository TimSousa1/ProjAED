#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

#define uint unsigned int
#define ushort unsigned short

//Simple struct for coordinates
typedef struct Vector2 {
    int x;
    int y;
} Vector2;

//Struct of a list whose item is a single coordinate
typedef struct _vectorlist {
    Vector2 tile;
    
    struct _vectorlist *next;
} VectorList;

//Struct that has a list of all moves and the final score
typedef struct _solution {
    int score;
    VectorList *moves;
} Solution;

//Struct that carries all the important information for each problem
//Such as its size, its variant, how many colors it has and how many tiles of each tile and
//The matrix that holds all tiles and their respective id
typedef struct _board {

    int lines;
    int columns;

    int variant;

    uint *colors;
    int numColors;

    Vector2 **tiles;

} Board;

//Struct that holds all the important information for each play made
typedef struct _moveList {
    Vector2 tile;
    int score;

    VectorList *clusters;
    Board *board;

    struct _moveList *next;
    struct _moveList *previous;
} MoveList;

//file.c
Board *getBoard(FILE*, int *error);
char *outputName(char *inputName);
void writeFile(FILE *file, Vector2 boardSize, int variant, Solution answer);

//board.c
Board *copyBoard(MoveList*);
void freeBoard(Board *board);
void countColors(Board *board);
int findCluster(Board*, int line, int column, int color, int id);
int blastCluster(Board*, int line, int column, int color, int id);
VectorList *findAllClusters(Board*);
MoveList *removeCluster(MoveList *lastMove, Vector2 tile);
void resetClusterSets(Board*);
void applyGravity(Board*);
uint hopeless(Board *board, int goal, MoveList *head);

//move.c
VectorList *addToVectorList(VectorList *head, Vector2 tile);
Solution solve(Board*);
void freeMoveList(MoveList *head);
void freeVectorList(VectorList *head);




void showBoard(Board*);
void showID(Board*);
void showMoveList(MoveList *lastMove);
void showVectorList(VectorList*);


#endif
