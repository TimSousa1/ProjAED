#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

uint convert(int line, int column, int maxColumn);

void countColors(Board *board) {

    for (uint line = 0; line < board->lines; line++) {
        for (uint column = 0; column < board->columns; column++) {
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
Vector2 findCluster(Board *board, int line, int column, int clusterColor, int originalID, bool toRemove){
    Vector2 ret = {.x = 0, .y = 0b00};
   // printf("finding cluster on tile (%i %i)..\n", line, column);
    if (line > board->lines || line <= 0 ||
            column > board->columns || column <= 0) return ret;

    int color;
    color = board->tiles[line - 1][column - 1].x;
    
    if (color < 1) return ret;
    if (color != clusterColor){
        ret.y = 0b11;
        return ret;
    }
    ret.x = 1; // tilesInCluster = 1
    board->tiles[line-1][column-1].y = originalID;
    //printf("adding tile to clusterSet..\n");

    // temporarily making the tile unavailable
    board->tiles[line-1][column-1].x = -1;

    Vector2 found = {.x = 0, .y = 0};
    found = findCluster(board, line + 1, column, clusterColor, originalID, toRemove); // up
    ret.x += found.x;
    ret.y = ret.y | found.y;

    found = findCluster(board, line - 1, column, clusterColor, originalID, toRemove); // down
    ret.x += found.x;
    ret.y = ret.y | found.y;

    found = findCluster(board, line, column - 1, clusterColor, originalID, toRemove); // left
    ret.x += found.x;
    ret.y = ret.y | found.y;

    found = findCluster(board, line, column + 1, clusterColor, originalID, toRemove); //right
    ret.x += found.x;
    ret.y = ret.y | found.y;

    // reassigning the tile its original color
    if (!toRemove) board->tiles[line-1][column-1].x = color;
    else board->colors[color-1]--;

    return ret;
}

VectorList *findAllClusters(Board* board){
    //printf("initializing search for all clusters..\n");
    int tilesInCluster;
    int color;
    int id;
    VectorList *head = NULL, *cluster = NULL;
    Vector2 clusterOut = {.x = 0, .y = 0};
    //showBoard(board);
    for (uint line = board->lines; line > 0; line--){
        for (uint column = 1; column <= board->columns; column++){
            //showBoard(board);
            //printf("on tile (%i %i)\n", column, line);
            color = board->tiles[line-1][column-1].x;
            if (color == -1) {/*printf("empty tile! moving on..\n");*/continue;}
            id = convert(line, column, board->columns);
            //printf("id %i\n", id);
            if (id == board->tiles[line-1][column-1].y && color != -1){
                //printf("tile not on a clusterSet!\n");
                clusterOut = findCluster(board, line, column, color, id, 0); // finding a single cluster
                if (clusterOut.x > 1) {
                    head = addToVectorList(head, (Vector2 ) {column, line});
                }
            }
        }
    }
    //showVectorList(head);
    return head;
}

uint convert(int line, int column, int maxColumn){
    return (line - 1) * maxColumn + column -1;
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
        move->previous = NULL;
        lastMove->previous = move;
    } else {
        move = lastMove->previous;
        freeVectorList(move->clusters);
    }

    for (uint line = 1; line <= copy->lines; line++) {
        for (uint column = 1; column <= copy->columns; column++) {
            if (copy->tiles[line-1][column-1].y == id) {
                copy->tiles[line-1][column-1].x = -1;
                tilesInCluster++;
                copy->colors[color-1]--;
            }
        }
    }
    
    move->tile = tile;
    move->id = id;
    move->color = color;

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

    for (uint line = 1; line <= board->lines; line++) {
        for (uint column = 1; column <= board->columns; column++) {
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

    uint score = 0;
    if (head) score = head->score; 

    for (uint i = 0; i < board->numColors; i++) {
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
        copied->colors = (uint*) calloc (toCopy->numColors, sizeof(uint));
        copied->tiles = (Vector2 **) malloc (toCopy->lines * sizeof(Vector2 *));

        for (uint k = 0; k < toCopy->lines; k++) {
            copied->tiles[k] = (Vector2 *) malloc (toCopy->columns * sizeof(Vector2));
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

void showTileList(TileList *tiles) {

    TileList *aux;
    for (aux = tiles; aux; aux = aux->next) {
        printf("%i %i\n%i %i\n\n", aux->initPos.y, aux->initPos.x, aux->finalPos.y, aux->finalPos.x);
    }
}

void showVectorList(VectorList *head){


    VectorList *aux;
    for (aux = head; aux; aux = aux->next) {
        printf("%i %i\n", aux->tile.x, aux->tile.y);
    }
    printf("------\n");
}
