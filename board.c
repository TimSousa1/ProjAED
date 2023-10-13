#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

uint convert(int line, int column, int maxColumn);
void resetClusterSets(uint *clusterSets, uint maxSize);
void applyGravity(Board *board);

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
    //printf("finding cluster on tile (%i %i)..\n", line, column);
    if (line > board->lines || line <= 0 ||
            column > board->columns || column <= 0)
    {/*printf("tile out of bounds!\n");*/return 0;}

    int color;
    color = board->tiles[line - 1][column - 1];
    
    // checking if:
    // the color is the same
    // already belongs in the same cluster
    // its the first tile being checked
    uint id = convert(line, column, board->columns);
    if (color != clusterColor || color == -1)
    {/*printf("tile already on set or empty!\n");*/return 0;}

    int tilesInCluster;
    tilesInCluster = 1;
    board->clusterSets[id] = originalID;
    //printf("adding tile to clusterSet..\n");

    // temporarily making the tile unavailable
    board->tiles[line-1][column-1] = -1;

    tilesInCluster += findCluster(board, line + 1, column, clusterColor, originalID); // up
    tilesInCluster += findCluster(board, line - 1, column, clusterColor, originalID); // down
    tilesInCluster += findCluster(board, line, column - 1, clusterColor, originalID); // left
    tilesInCluster += findCluster(board, line, column + 1, clusterColor, originalID); //right

    // reassigning the tile its original color
    board->tiles[line-1][column-1] = color;

    return tilesInCluster;
}

int findAllClusters(Board* board){
    //printf("initializing search for all clusters..\n");
    int tilesInCluster, biggestId = -1;
    int color;
    uint id, biggestCluster = 1;

    for (uint line = board->lines; line > 0; line--){
        for (uint column = 1; column <= board->columns; column++){
            //printf("on tile (%i %i)\n", line, column);
            color = board->tiles[line-1][column-1];
            id = convert(line, column, board->columns);
            //printf("if of current tile is %u\n", id);
            if (id == board->clusterSets[id]){
                //printf("tile not on a clusterSet!\n");
                tilesInCluster = findCluster(board, line, column, color, id);
                if (tilesInCluster > biggestCluster) { 
                    biggestCluster = tilesInCluster;
                    biggestId = id;
                }
            }
        }
    }
    return biggestId;
}

uint convert(int line, int column, int maxColumn){
    return (line - 1) * maxColumn + column -1;
}

uint removeCluster(Board *board, uint line, uint column) {

    uint id = board->clusterSets[convert(line, column, board->columns)];
    uint totalTiles = 0;

    for (uint line = 1; line <= board->lines; line++) {
        for (uint column = 1; column <= board->columns; column++) {
            if (board->clusterSets[convert(line, column, board->columns)] == id) {
                board->tiles[line - 1][column - 1] = -1;
                totalTiles++;
            }
        }
    }
    resetClusterSets(board->clusterSets, board->lines * board->columns);
    applyGravity(board);
    return totalTiles;
}

void resetClusterSets(uint *clusterSets, uint maxSize) {
    for (uint id = 0; id < maxSize; id++) clusterSets[id] = id;
}

void applyGravity(Board *board) {

    int line, column, counter;
    
    /* Simulating vertical gravity */
    for (column = 0; column < board->columns; column++) {
        /* Setting the counter of empty tiles to 0 */
        counter = 0;
        for (line = 0; line < board->lines; line++) {

            /* Counting the number of empty tiles */
            if (board->tiles[line][column] == -1) {
                counter++;
            /* Making a tile fall */
            } else if (counter) {
                board->tiles[line - counter][column] = board->tiles[line][column];
                board->tiles[line][column] = -1;
            }
        }
    }

    /* Simulating horizontal gravity */
    counter = 0;
    for (column = board->columns - 1; column >= 0; column--) {
        /* Counting the number of empty columns */
        if (board->tiles[0][column] == -1) {
            counter++;
        /* Sliding a column to the right */
        } else if (counter) {
            for (line = 0; line < board->lines; line++) {
                board->tiles[line][column + counter] = board->tiles[line][column];
                board->tiles[line][column] = -1;
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
    free(board->clusterSets);
    /* Freeing the memory allocated for the board */
    free(board);

    return;
}

void showBoard(Board *board){
    for (int i = board->lines - 1; i >= 0; i--) {
        for (int j = 0; j < board->columns; j++) {
            fprintf(stdout, "%i ", board->tiles[i][j]);
        }
        fprintf(stdout, "\n");
    } 
    fprintf(stdout, "\n");
}

void showID(Board* board){
    for (int line = board->lines; line > 0; line--) {
        for (int column = 1; column <= board->columns; column++) {
            fprintf(stdout, "%4i ", board->clusterSets[convert(line, column, board->columns)]);
        }
        fprintf(stdout, "\n");
    } 
    fprintf(stdout, "\n");
}
