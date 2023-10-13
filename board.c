#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>


uint convert(int line, int column, int maxColumn);

/******************************************************************************
 * findCluster ()
 *
 * Arguments: Board *board, int line and int column
 * Returns: CellList *
 * Side-Effects: Changes the clusterSets of the first argument (Board *)
 *
 * Description: Groups the tile provided as an argument in a cluster
 *****************************************************************************/

int findCluster(Board *board, int line, int column, int clusterColor, uint originalID){
    printf("finding cluster on tile (%i %i)..\n", line, column);
    if (line > board->lines || line <= 0 ||
            column > board->columns || column <= 0)
    {/*printf("tile out of bounds!\n");*/return 0;}

    int color;
    color = board->tiles[line - 1][column - 1].x;
    
    if (color != clusterColor || color == -1)
    {/*printf("tile already on set or empty!\n");*/return 0;}

    int tilesInCluster;
    tilesInCluster = 1;
    board->tiles[line - 1][column - 1].y = originalID;
    printf("adding tile to clusterSet..\n");

    // temporarily making the tile unavailable
    board->tiles[line-1][column-1].x = -1;

    tilesInCluster += findCluster(board, line + 1, column, clusterColor, originalID); // up
    tilesInCluster += findCluster(board, line - 1, column, clusterColor, originalID); // down
    tilesInCluster += findCluster(board, line, column - 1, clusterColor, originalID); // left
    tilesInCluster += findCluster(board, line, column + 1, clusterColor, originalID); //right

    // reassigning the tile its original color
    board->tiles[line-1][column-1].x = color;

    return tilesInCluster;
}

void findAllClusters(Board* board){
    printf("initializing search for all clusters..\n");
    int tilesInCluster;
    int color;
    uint id;

    for (uint line = board->lines; line > 0; line--){
        for (uint column = 1; column <= board->columns; column++){
            printf("on tile (%i %i)\n", line, column);
            color = board->tiles[line-1][column-1].x;
            id = convert(line, column, board->columns);
            printf("if of current tile is %u\n", id);
            if (id == board->tiles[line-1][column-1].y){
                printf("tile not on a clusterSet!\n");
                tilesInCluster = findCluster(board, line, column, color, id);
            }
        }
    }
    return;
}

// searches for the fist cluster from top left to right
int findTopSweep(Board* board){
    int color, id;
    int tilesInCluster;

    for (uint line = board->lines; line > 0; line--){
        for (uint column = 1; column <= board->columns; column++){
            color = board->tiles[line-1][column-1].x;
            id = board->tiles[line-1][column-1].y;
            tilesInCluster = findCluster(board, line, column, color, id);
            if (tilesInCluster > 1) return id;
        }
    }
    return -1;
}

// searches for the first cluster from bottom left to right
int findBottomSweep(Board* board){
    int color, id;
    int tilesInCluster;

    for (uint line = 1; line <= board->lines; line++){
        for (uint column = 1; column <= board->columns; column++){
            color = board->tiles[line-1][column-1].x;
            id = board->tiles[line-1][column-1].y;
            tilesInCluster = findCluster(board, line, column, color, id);
            if (tilesInCluster > 1) return id;
        }
    }
    return -1;
}

int findLargest(Board* board){
    int color, id, largerID = -1;
    int tilesInCluster, tilesInBiggestCluster = 1;

    for (uint line = 1; line <= board->lines; line++){
        for (uint column = 1; column <= board->columns; column++){
            color = board->tiles[line-1][column-1].x;
            id = board->tiles[line-1][column-1].y;

            tilesInCluster = findCluster(board, line, column, color, id);
            if (tilesInCluster > tilesInBiggestCluster){
                tilesInBiggestCluster = tilesInCluster;
                largerID = id;
            }
        }
    }
    return largerID;
}

uint convert(int line, int column, int maxColumn){
    return (line - 1) * maxColumn + column -1;
}

uint removeCluster(Board *board, int id) {

    uint totalTiles = 0;

    for (uint line = 1; line <= board->lines; line++) {
        for (uint column = 1; column <= board->columns; column++) {
            if (board->tiles[line-1][column-1].y == id) {
                board->tiles[line - 1][column - 1].x = -1;
                totalTiles++;
            }
        }
    }
    return totalTiles;
}

void resetClusterSets(Board* board) {
    for (uint line = 1; line <= board->lines; line++) {
        for (uint column = 1; column <= board->columns; column++) {
            board->tiles[line-1][column-1].y = convert(line, column, board->columns);
        }
    }
}

void applyGravity(Board *board) {

    int line, column, counter;
    
    /* Simulating vertical gravity */
    for (column = 0; column < board->columns; column++) {
        /* Setting the counter of empty tiles to 0 */
        counter = 0;
        for (line = 0; line < board->lines; line++) {

            /* Counting the number of empty tiles */
            if (board->tiles[line][column].x == -1) {
                counter++;
            /* Making a tile fall */
            } else if (counter) {
                board->tiles[line - counter][column].x = board->tiles[line][column].x;
                board->tiles[line][column].x = -1;
            }
        }
    }

    /* Simulating horizontal gravity */
    counter = 0;
    for (column = board->columns - 1; column >= 0; column--) {
        /* Counting the number of empty columns */
        if (board->tiles[0][column].x == -1) {
            counter++;
        /* Sliding a column to the right */
        } else if (counter) {
            for (line = 0; line < board->lines; line++) {
                board->tiles[line][column + counter].x = board->tiles[line][column].x;
                board->tiles[line][column].x = -1;
            }
        }
    }
    return;
}


void freeBoard(Board *board) {

    int i;
    /* Freeing the memory allocated for the tiles matrix */
    for (i = 0; i < board->lines; i++) {
        free(board->tiles[i]);
    }
    free(board->tiles);
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
