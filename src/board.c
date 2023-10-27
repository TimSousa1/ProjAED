#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * convert()
 *
 * Arguments: int line, int column and int maxColumn
 * Returns: uint <nameless>
 * Side-Effects: none
 *
 * Description: Calculates the equivalent id for a coordinate
 *****************************************************************************/

uint convert(int line, int column, int maxColumn){
    return (line - 1) * maxColumn + column -1;
}

/******************************************************************************
 * countColors()
 *
 * Arguments: Board *board
 * Returns: nothing
 * Side-Effects: none
 *
 * Description: Calculates the equivalent id for a coordinate
 *****************************************************************************/

void countColors(Board *board) {

    for (int line = 0; line < board->lines; line++) {
        for (int column = 0; column < board->columns; column++) {
            if (board->tiles[line][column].x > 0)
                board->colors[board->tiles[line][column].x - 1]++;
        }
    }
}

Vector2 convertToCoordinates(int id, int columns) {
    Vector2 ret;
    ret.x = id % columns + 1;
    ret.y = id / columns + 1;
    return ret;
}

VectorList *mergeVectorList(VectorList *first, VectorList *second) {
    if (!first || !second) return first;
    //printf("merging %i %i with %i %i\n", first->tile.x, first->tile.y, second->tile.x, second->tile.y );

    VectorList *last;

    for (last = first; last->next;) last = last->next;
    last->next = second;

    return first;
}

// .x = number of tiles in the cluster
// .y = 0bxy
//        x .. vertical gravity
//        y .. horizontal gravity
int findCluster(Board *board, int line, int column, int clusterColor, int originalID){
   // printf("finding cluster on tile (%i %i)..\n", line, column);

    int color;
    color = board->tiles[line - 1][column - 1].x;
    
    if (color < 1) return 0;

    int tilesInCluster = 1; // tilesInCluster = 1
    board->tiles[line-1][column-1].y = originalID;
    //printf("adding tile to clusterSet..\n");

    // temporarily making the tile unavailable
    board->tiles[line-1][column-1].x = -1;

    if (line+1 <= board->lines){
        if (board->tiles[line - 1+1][column - 1].x == clusterColor) 
            tilesInCluster += findCluster(board, line + 1, column, clusterColor, originalID); // up
    }
    if (line-1 > 0 ){
        if (board->tiles[line - 1-1][column - 1].x == clusterColor) 
            tilesInCluster += findCluster(board, line - 1, column, clusterColor, originalID); // down
    }
    if (column-1 > 0){
        if (board->tiles[line - 1][column - 1-1].x == clusterColor) 
            tilesInCluster += findCluster(board, line, column - 1, clusterColor, originalID); // left
    }
    if (column+1 <= board->columns ){
        if (board->tiles[line - 1][column - 1+1].x == clusterColor) 
            tilesInCluster += findCluster(board, line, column + 1, clusterColor, originalID); //right
    }

    // reassigning the tile its original color
    board->tiles[line-1][column-1].x = color;

    return tilesInCluster;
}

int blastCluster(Board *board, int line, int column, int clusterColor, int originalID){
   // printf("finding cluster on tile (%i %i)..\n", line, column);

    int color;
    color = board->tiles[line - 1][column - 1].x;
    
    if (color < 1) return 0;

    int tilesInCluster = 1; // tilesInCluster = 1
    board->tiles[line-1][column-1].y = originalID;
    //printf("adding tile to clusterSet..\n");

    // temporarily making the tile unavailable
    board->tiles[line-1][column-1].x = -1;

    if (line+1 <= board->lines){
        if (board->tiles[line - 1+1][column - 1].x == clusterColor) 
            tilesInCluster += blastCluster(board, line + 1, column, clusterColor, originalID); // up
    }
    if (line-1 > 0 ){
        if (board->tiles[line - 1-1][column - 1].x == clusterColor) 
            tilesInCluster += blastCluster(board, line - 1, column, clusterColor, originalID); // down
    }
    if (column-1 > 0){
        if (board->tiles[line - 1][column - 1-1].x == clusterColor) 
            tilesInCluster += blastCluster(board, line, column - 1, clusterColor, originalID); // left
    }
    if (column+1 <= board->columns ){
        if (board->tiles[line - 1][column - 1+1].x == clusterColor) 
            tilesInCluster += blastCluster(board, line, column + 1, clusterColor, originalID); //right
    }

    return tilesInCluster;
}
VectorList *findAllClusters(Board* board){
    //printf("initializing search for all clusters..\n");
    int tilesInCluster;
    int color;
    int id;
    VectorList *head = NULL;
    //showBoard(board);
    for (int column = board->columns; column > 0; column--){
        if (board->tiles[0][column-1].x == -1) break;
        for (int line = 1; line <= board->lines; line++){
            //showBoard(board);
            //printf("on tile (%i %i)\n", column, line);
            color = board->tiles[line-1][column-1].x;
            if (color == -1) {/*printf("empty tile! moving on..\n");*/break;}
            id = convert(line, column, board->columns);
            //printf("id %i\n", id);
            if (id == board->tiles[line-1][column-1].y && color != -1){
                //printf("tile not on a clusterSet!\n");
                tilesInCluster = findCluster(board, line, column, color, id); // finding a single cluster
                if (tilesInCluster > 1) {
                    head = addToVectorList(head, (Vector2 ) {column, line});
                }
            }
        }
    }
    //showVectorList(head);
    return head;
}

MoveList *removeCluster(MoveList *lastMove, Vector2 tile) {

    //printf("removing cluster at %i %i\n", tile.x, tile.y);
    Board *copy = copyBoard(lastMove);
    uint tilesInCluster = 0;
    int color, id;
    color = copy->tiles[tile.y-1][tile.x-1].x;
    id = copy->tiles[tile.y-1][tile.x-1].y;

    MoveList *move = NULL;

    if (!lastMove->previous) {
        move = (MoveList *) malloc(sizeof(MoveList));
        if (!move) {
            fprintf(stderr, "Error allocating memory for variable \"move\", in function removeCluster()!\n");
            exit(1);
        }
        move->previous = NULL;
        lastMove->previous = move;
    } else {
        move = lastMove->previous;
        freeVectorList(move->clusters);
    }

    tilesInCluster = blastCluster(copy, tile.y, tile.x, color, id);
    copy->colors[color-1] -= tilesInCluster;
    
    move->tile = tile;

    move->score = tilesInCluster * (tilesInCluster - 1);
    move->next = lastMove;
    
    move->clusters = NULL;

    applyGravity(copy);
    resetClusterSets(copy);
    move->clusters = findAllClusters(copy);

    move->board = copy;
    //printf("board after removal..\n");
    //showBoard(move->board);
    return move;
}

void resetClusterSets(Board* board) {
    //printf("board reset requested for board:\n");
    //showBoard(board);

    //printf("with id:\n");
    //showID(board);

    for (int line = 1; line <= board->lines; line++) {
        for (int column = 1; column <= board->columns; column++) {
            board->tiles[line-1][column-1].y = convert(line, column, board->columns);
            //printf("resetting tile %i %i back to id %i\n", column, line, board->tiles[line-1][column-1].y);
        }
    }
}

void applyVerticalGravity(Board *board){

    int line, column, counter;
    
    /* Simulating vertical gravity */
    for (column = 1; column <= board->columns; column++) {
        /* Setting the counter of empty tiles to 0 */
        counter = 0;
        for (line = 1; line <= board->lines; line++) {

            /* Counting the number of empty tiles */
            if (board->tiles[line-1][column-1].x < 0) {
                counter++;
            /* Making a tile fall */
            } else if (counter) {
                //printf("%p\n", headTile);
                board->tiles[line - counter-1][column-1].x = board->tiles[line-1][column-1].x;
                board->tiles[line-1][column-1].x = -1;
            }
        }
    }

}

void applyHorizontalGravity(Board *board){

    int line, column, counter;

    /* Simulating horizontal gravity */
    counter = 0;
    for (column = board->columns; column > 0; column--) {
        /* Counting the number of empty columns */
        if (board->tiles[0][column-1].x < 0) {
            counter++;
        /* Sliding a column to the right */
        } else if (counter) {
            for (line = 1; line <= board->lines; line++) {
                board->tiles[line-1][column-1 + counter].x = board->tiles[line-1][column-1].x;
                board->tiles[line-1][column-1].x = -1;
            }
        }
    }
}

void applyGravity(Board *board) {
    applyVerticalGravity(board);
    applyHorizontalGravity(board);
}

uint hopeless(Board *board, int goal, MoveList *head) {

    if (goal < 0) return 0;

    int score = 0;
    if (head) score = head->score; 

    for (int i = 0; i < board->numColors; i++) {
        score += board->colors[i] * (board->colors[i]-1);
    }
    //printf("Max Possible score: %i\n", score);
    if (score < goal) return 1;
    //printf("Theres Hope!\n");
    return 0;
}

Board *copyBoard(MoveList *move){
    //printf("copying board..\n");
    //showBoard(toCopy);
    Board *toCopy = NULL, *copied = NULL;
    toCopy = move->board;

    if (!move->previous) {
        copied = (Board*) malloc (sizeof(*copied));
        if (!copied) {
            fprintf(stderr, "Error allocating memory for variable \"copied\", in function copyBoard()!\n");
            exit(1);
        }
        copied->colors = (uint*) calloc (toCopy->numColors, sizeof(uint));
        if (!copied->colors) {
            fprintf(stderr, "Error allocating memory for variable \"copied->colors\", in function copyBoard()!\n");
            exit(1);
        }
        copied->tiles = (Vector2 **) malloc (toCopy->lines * sizeof(Vector2 *));
        if (!copied->tiles) {
            fprintf(stderr, "Error allocating memory for variable \"copied->tiles\", in function copyBoard()!\n");
            exit(1);
        }

        for (int k = 0; k < toCopy->lines; k++) {
            copied->tiles[k] = (Vector2 *) malloc (toCopy->columns * sizeof(Vector2));
            if (!copied->tiles[k]) {
                fprintf(stderr, "Error allocating memory for variable \"copied->tiles[%i]\", in function copyBoard()!\n", k);
                exit(1);
            }
        }
    }
    else {
        copied = move->previous->board;
    }

    copied->lines = toCopy->lines;
    copied->columns = toCopy->columns;
    copied->variant = toCopy->variant;
    copied->numColors = toCopy->numColors;


    for (int i = 0; i < toCopy->numColors; i++)
        copied->colors[i] = toCopy->colors[i];

    for (int line = 0; line < toCopy->lines; line++){
        for (int column = 0; column < toCopy->columns; column++){
            copied->tiles[line][column] = toCopy->tiles[line][column];
        }
    }
    //printf("resulting board..\n");
    //showBoard(copied);
    return copied;
}

void freeBoard(Board *board) {

    int i;
    /* Freeing the memory allocated for the tiles matrix */
    for (i = 0; i < board->lines; i++) {
        free(board->tiles[i]);
    }
    free(board->tiles);
    free(board->colors);
    /* Freeing the memory allocated for the board */
    free(board);

    return;
}

void showBoard(Board *board){
    for (int i = board->lines - 1; i >= 0; i--) {
        for (int j = 0; j < board->columns; j++) {
            fprintf(stdout, "%4i ", board->tiles[i][j].x);
        }
        fprintf(stdout, "\n");
    } 
    fprintf(stdout, "\n");
    fprintf(stdout, "columns: %i lines: %i variant: %i numOfColors: %i\n",
            board->columns, board->lines, board->variant, board->numColors);
}

void showID(Board* board){
    for (int line = board->lines; line > 0; line--) {
        for (int column = 1; column <= board->columns; column++) {
            fprintf(stdout, "%4i ", board->tiles[line-1][column-1].y);
        }
        fprintf(stdout, "\n");
    } 
    fprintf(stdout, "\n");
}

void showVectorList(VectorList *head){


    VectorList *aux;
    for (aux = head; aux; aux = aux->next) {
        printf("%i %i\n", aux->tile.x, aux->tile.y);
    }
    printf("------\n");
}
