#ifndef _TILES
#define _TILES

#include <stdio.h>
#include <stdlib.h>

#define uint unsigned int
#define ushort unsigned short

// a pair of vectors
typedef struct Vector2 {
    int x;
    int y;
} Vector2;

// a list of Vector2s
typedef struct _vectorlist {
    Vector2 tile;
    struct _vectorlist *next;
} VectorList;

// a list of all the moves made and the final score
typedef struct _solution {
    int score;         // the score
    VectorList *moves; // the moves for this solution
} Solution;

// a board holding all the info related to the matrix of tiles
typedef struct _board {
    int lines;       // the number of tiles the board has
    int columns;     // the number of columns
    int variant;     // the variant it's being solved for
    uint *colors;    // a vector holding how many of each color there is in the board
    int numColors;   // the total number of colors
    Vector2 **tiles; // the actual matrix of Vector2 holding the value of each value's color and if
} Board;

// a stack holding all the moves made
typedef struct _moveList {
    Vector2 tile;               // blasted tile
    int score;                  // current score
    VectorList *clusters;       // possible plays 
    Board *board;               // the move's board
                                //
    struct _moveList *next;     // the next element in the stack (previous move)
    struct _moveList *previous; // the previous element in the stack (next move)
} MoveList;

//file.c
Board *getBoard(FILE*, int *error);                                          // gets a single board from the input file 
char *outputName(char *inputName);                                           // generates the output name of the output file
void writeFile(FILE *file, Vector2 boardSize, int variant, Solution answer); // writes a list of moves to a file

//board.c
Board *copyBoard(MoveList*);                                       // creates a copy of a board from the specified move
void freeBoard(Board *board);                                      // frees a board
void countColors(Board *board);                                    // counts the number of colors on a board
int findCluster(Board*, int line, int column, int color, int id);  // returns the number of tiles in a cluster at line, column coordinates
int blastCluster(Board*, int line, int column, int color, int id); // blasts (removes) a cluster at the specified coordinates
VectorList *findAllClusters(Board*);                               // find all the clusters on a board, return a list of possible moves
MoveList *removeCluster(MoveList *lastMove, Vector2 tile);         // creates a new move from the removal of a cluster
void resetClusterSets(Board*);                                     // resets the identifier of each tile
void applyGravity(Board*);                                         // applies gravity to the board
uint hopeless(Board *board, int goal, MoveList *head);             // checks if it's worth keeping on going

//move.c
VectorList *addToVectorList(VectorList *head, Vector2 tile); // adds a Vector2 to a stack of Vector2s
Solution solve(Board*);                                      // solves the whole board for it's variant
void freeMoveList(MoveList *head);                           // frees a MoveList
void freeVectorList(VectorList *head);                       // frees a VectorList;

// printing
void showBoard(Board*);                // prints a board to stdout
void showID(Board*);                   // prints the id of each tile 
void showMoveList(MoveList *lastMove); // prints all the members in a MoveList
void showVectorList(VectorList*);      // prints all the members of a VectorList

#endif
