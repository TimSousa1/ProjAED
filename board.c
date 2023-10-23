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

/******************************************************************************
 * findCluster ()
 *
 * Arguments: Board *board, int line and int column
 * Returns: CellList *
 * Side-Effects: Changes the clusterSets of the first argument (Board *)
 *
 * Description: Groups the tile provided as an argument in a cluster
 *****************************************************************************/

VectorList *findCluster(Board *board, int line, int column, int clusterColor, int originalID, bool toRemove) {
    //printf("finding cluster on tile (%i %i)..\n", line, column);
    if (line > board->lines || line <= 0 ||
            column > board->columns || column <= 0)
    {/*printf("tile (%i %i) out of bounds!\n", column, line);*/return NULL;}

    int color;
    color = board->tiles[line - 1][column - 1].x;
    
    if (color != clusterColor || color < 1)
    {/*printf("tile (%i %i) already on set or empty!\n", column, line);*/return NULL;}

    VectorList *tile = (VectorList *) malloc(sizeof(VectorList));
    tile->next = NULL;
    tile->tile = (Vector2) {column, line};
    //printf("adding tile (%i %i) to clusterSet..\n", column, line);

    // temporarily making the tile unavailable
    board->tiles[line-1][column-1].x = -1;
    board->tiles[line-1][column-1].y = originalID;

    VectorList *newTile, *tmp;
    newTile = findCluster(board, line + 1, column, clusterColor, originalID, toRemove); // up
    /*if (newTile) printf("got a new tile at %i %i\n", newTile->tile.x, newTile->tile.y);
    else printf("no eligible tile found\n");*/
    tile = mergeVectorList(tile, newTile);

    newTile = findCluster(board, line - 1, column, clusterColor, originalID, toRemove); // down
    tile = mergeVectorList(tile, newTile);

    newTile = findCluster(board, line, column - 1, clusterColor, originalID, toRemove); // left
    tile = mergeVectorList(tile, newTile);

    newTile = findCluster(board, line, column + 1, clusterColor, originalID, toRemove); //right
    tile = mergeVectorList(tile, newTile);

    // reassigning the tile its original color
    if (!toRemove) board->tiles[line-1][column-1].x = color;
    else board->colors[color-1]--;

    return tile;
}

#if 0

VectorList *findAllClusters(Board* board){
    return NULL;
}

#else
// we should seriously consider stop using thishelle
VectorList *findAllClusters(Board* board){
    //printf("initializing search for all clusters..\n");
    int tilesInCluster;
    int color;
    int id;
    VectorList *head = NULL, *cluster = NULL;

    for (uint line = board->lines; line > 0; line--){
        for (uint column = 1; column <= board->columns; column++){
            //showID(board);
            //printf("on tile (%i %i)\n", line, column);
            color = board->tiles[line-1][column-1].x;
            id = convert(line, column, board->columns);
            //printf("id %i\n", id);
            if (id == board->tiles[line-1][column-1].y && color != -1){
                //printf("tile not on a clusterSet!\n");
                cluster = findCluster(board, line, column, color, id, 0); // finding a single cluster
                if (cluster->next) {
                    head = addToVectorList(head, cluster->tile);
                }
                freeVectorList(cluster); 
            }
        }
    }
    return head;
}
#endif

#ifdef SWEEPS

// searches for the fist cluster from top left to right
int findTopSweep(Board* board){
    int color, id;
    int tilesInCluster;

    for (uint line = board->lines; line > 0; line--){
        for (uint column = 1; column <= board->columns; column++){
            color = board->tiles[line-1][column-1].x;
            id = board->tiles[line-1][column-1].y;
            tilesInCluster = findCluster(board, line, column, id, 0);
            if (tilesInCluster > 1) return id;
        }
    }
    return -1;
}

int idSweep(Board *board, int lastID) {
    Vector2 tile;
    int tilesInCluster, column, line, color;

    for (int id = lastID+1; id < board->columns*board->lines; id++) {
        tile = convertToCoordinates(id, board->columns);
        if (board->tiles[tile.y-1][tile.x-1].x == -1 ||
            board->tiles[tile.y-1][tile.x-1].y != id) continue;

        color = board->tiles[tile.y-1][tile.x-1].x;
        tilesInCluster = findCluster(board, tile.y, tile.x, color, id);

        if (tilesInCluster > 1) return id;
    }
    return -1;
}

// searches for the first cluster from bottom left to right
Vector2 findBottomSweep(Board* board, Vector2 play){
    int color, id;
    int tilesInCluster;
    Vector2 ret;
    ret.x = -1;
    ret.y = -1;

    for (uint line = play.y + 1; line <= board->lines; line++){
        for (uint column = play.x + 1; column <= board->columns; column++){
            color = board->tiles[line-1][column-1].x;
            id = board->tiles[line-1][column-1].y;
            tilesInCluster = findCluster(board, line, column, color, id, 0);
            if (tilesInCluster > 1) {
                ret.x = column - 1;
                ret.y = line - 1;
                return ret;
            }
        }
    }
    return ret;
}

int findLargest(Board* board){
    int color, id, largerID = -1;
    int tilesInCluster, tilesInBiggestCluster = 1;

    for (uint line = 1; line <= board->lines; line++){
        for (uint column = 1; column <= board->columns; column++){
            color = board->tiles[line-1][column-1].x;
            id = board->tiles[line-1][column-1].y;

            tilesInCluster = findCluster(board, line, column, color, id, 1);
            if (tilesInCluster > tilesInBiggestCluster){
                tilesInBiggestCluster = tilesInCluster;
                largerID = id;
            }
        }
    }
    return largerID;
}
#endif

uint convert(int line, int column, int maxColumn){
    return (line - 1) * maxColumn + column -1;
}

MoveList *removeCluster(Board *board, Vector2 tile) {

    uint totalTiles = 0;
    int color;
    MoveList *move = (MoveList *) malloc(sizeof(MoveList));
    move->removedTiles = NULL;
    color = board->tiles[tile.y-1][tile.x-1].x;
    
    move->removedTiles = findCluster(board, tile.y, tile.x, color, -1, 1);

    move->tile = tile;
    move->id = convert(tile.y, tile.x, board->columns);
    move->color = color;
    //showVectorList(move->removedTiles);
    for (VectorList *current = move->removedTiles; current; current = current->next) totalTiles++;
    //printf("??%i??\n", totalTiles);
    move->score = totalTiles * (totalTiles - 1);
    move->next = NULL;
    return move;
}

void resetClusterSets(Board* board) {
    for (uint line = 1; line <= board->lines; line++) {
        for (uint column = 1; column <= board->columns; column++) {
            board->tiles[line-1][column-1].y = convert(line, column, board->columns);
        }
    }
}

TileList *applyGravity(Board *board) {

    int line, column, counter;
    TileList *headTile = NULL;
    
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
                headTile = addToTileList(headTile, (Vector2) {column, line}, (Vector2) {column, line - counter});
                //printf("%p\n", headTile);
                board->tiles[line - counter-1][column-1].x = board->tiles[line-1][column-1].x;
                board->tiles[line-1][column-1].x = -1;
            }
        }
    }

    /* Simulating horizontal gravity */
    counter = 0;
    for (column = board->columns; column > 0; column--) {
        /* Counting the number of empty columns */
        if (board->tiles[0][column-1].x < 0) {
            counter++;
        /* Sliding a column to the right */
        } else if (counter) {
            for (line = 1; line <= board->lines; line++) {
                headTile = addToTileList(headTile, (Vector2) {column, line}, (Vector2) {column + counter, line});

                //We havent tried anything yet

                board->tiles[line-1][column-1 + counter].x = board->tiles[line-1][column-1].x;
                board->tiles[line-1][column-1].x = -1;
            }
        }
    }
    return headTile;
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
    //printf("%p\n", tiles);
    for (aux = tiles; aux; aux = aux->next) {
        printf("%i %i\n%i %i\n\n", aux->initPos.y, aux->initPos.x, aux->finalPos.y, aux->finalPos.x);
    }
}

void showVectorList(VectorList *head){

    VectorList *aux;
    //printf("%p\n", tiles);
    for (aux = head; aux; aux = aux->next) {
        printf("%i %i\n", aux->tile.x, aux->tile.y);
    }
}
