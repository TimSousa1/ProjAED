#include "common.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _cellList{
    int line;
    int column;
    int color;

    struct _cellList *next;
} CellList;

CellList *createCell(Board *board, int line, int column, ushort id, short color);
void listAdd(CellList *element, CellList *toAdd);
void showCell(CellList *cell);
void showCells(CellList *head);
void showID(ushort *id, int size);
void applyGravity(Board *board);

/******************************************************************************
 * findTileCluster ()
 *
 * Arguments: Board *, short and short
 * Returns: nothing
 * Side-Effects: Changes the clusterSets of the first argument (Board *)
 *
 * Description: Groups the tile provided as an argument in a cluster
 *****************************************************************************/

void findTileCluster(Board *board, short line, short column){
    ushort headID = board->clusterSets[(line - 1) * board->columns + column - 1];
    //printf("creating list with headID %i\n", headID);
    CellList *head = (CellList*) malloc (sizeof(CellList));
    CellList *current = NULL;

    head->line = line - 1;
    head->column = column - 1;
    head->color = board->tilesBoard[line - 1][column - 1];
    head->next = NULL;

    // TODO: optimize? take ifs from createCell and listAdd and move them to the loop
    for (current = head; current;){

        listAdd(current, createCell(board, current->line + 1, current->column, headID, head->color)); // up
        listAdd(current, createCell(board, current->line - 1, current->column, headID, head->color)); // down
        listAdd(current, createCell(board, current->line, current->column - 1, headID, head->color)); // left
        listAdd(current, createCell(board, current->line, current->column + 1, headID, head->color)); // right

        // should only run if for condition is met
        current = current->next;
        free(head);
        head = current;
        //showCells(head);
    }
    //showID(board->clusterSets, board->lines * board->columns);

    return;
}

/******************************************************************************
 * createCell ()
 *
 * Arguments: Board *, int, int, ushort, short
 * Returns: CellList *
 * Side-Effects: Creates a CellList *
 *
 * Description: Creates a Cell if its tile has the same color as the original tile
 *****************************************************************************/

CellList *createCell(Board *board, int line, int column, ushort id, short color){
    //printf("creating cell..\n checking cell boundaries..\n");
    if (line > board->lines - 1 || line < 0 ||
            column > board->columns - 1 || column < 0){
        //printf("cell out of bounds!\n");
        return NULL;
    }
    if (board->tilesBoard[line][column] != color) return NULL;

    ushort i = line * board->columns + column;

    if (board->clusterSets[i] == id) {/*printf("cell already on clusterSet\n");*/ return NULL;}
    board->clusterSets[i] = id;

    CellList *cell = (CellList*) malloc (sizeof(CellList));

    cell->line = line;
    cell->column = column;
    cell->color = board->tilesBoard[line][column];
    cell->next = NULL;
    //printf("cell created with attributes "); showCell(cell);

    return cell;
}

/******************************************************************************
 * listAdd ()
 *
 * Arguments: CellList * and CellList *
 * Returns: nothing
 * Side-Effects: Puts the second argument in the same list as the first after it
 *
 * Description: Adds a new element to a list 
 * *****************************************************************************/

void listAdd(CellList *element, CellList *toAdd){
    if (!toAdd) return;
     
    toAdd->next = element->next;
    element->next = toAdd;

    return;
}

void showCell(CellList *cell){
        printf("----\n%i (%i, %i)\n", cell->color, cell->line, cell->column);
}

void showCells(CellList *head){
    for (CellList *current = head; current; current = current->next){
        printf("----\n%i (%i, %i)\n", current->color, current->line, current->column);
    }
}

void showID(ushort *id, int size){
    printf("printing IDs..\n");
    for (ushort i = 0; i < size; i++) printf("%i ", id[i]);
    printf("\n");
}

void showBoard(Board *board){
    for (short i = board->lines - 1; i >= 0; i--){
        for (short j = 0; j < board->columns; j++){
            printf("%hi ", board->tilesBoard[i][j]);
        }
        printf("\n");
    }
}

/******************************************************************************
 * freeBoard ()
 *
 * Arguments: Board *
 * Returns: nothing
 * Side-Effects: Frees its argument
 *
 * Description: Frees the memory allocated for a Board
 * *****************************************************************************/

void freeBoard(Board *board) {

    short i;

    for (i = 0; i < board->lines; i++) {
        free(board->tilesBoard[i]);
    }
    free(board->tilesBoard);
    free(board->clusterSets);
    free(board);

    return;
    
}

/******************************************************************************
 * removeCluster ()
 *
 * Arguments: Board *
 * Returns: nothing
 * Side-Effects: Changes the board->tilesBoard by removing a cluster 
 * 
 * Description: Removes a cluster from a board->tilesBoard
 * *****************************************************************************/

void removeCluster(Board *board) {

    short cluster, i, j, k, loneTile;

    cluster = board->clusterSets[(board->l - 1) * board->columns + (board->c - 1)];

    loneTile = 1;
    for (i = 0; i < board->lines * board->columns; i++) {
        if (i != (board->l - 1) * board->columns + board->c - 1 && board->clusterSets[i] == cluster) loneTile = 0;
    }

    if (loneTile) return;

    for (i = 0; i < board->lines; i++) {
        for (j = 0; j < board->columns; j++) {
            if (board->clusterSets[i * board->columns + j] == cluster) {
                board->tilesBoard[i][j] = -1;
            }
        }
    }

    applyGravity(board);

    return;

}

/******************************************************************************
 * applyGravity ()
 *
 * Arguments: Board *
 * Returns: nothing
 * Side-Effects: Changes the board->tilesBoard as if gravity took effect to the tiles
 
 * Description: Adds a new element to a list 
 * *****************************************************************************/

void applyGravity(Board *board) {

    short line, column, counter;

    for (column = 0; column < board->columns; column++) {
        counter = 0;
        for (line = 0; line < board->lines; line++) {
            if (board->tilesBoard[line][column] == -1) {
                counter++;
            } else if (counter) {
                board->tilesBoard[line - counter][column] = board->tilesBoard[line][column];
                board->tilesBoard[line][column] = -1;
            }
        }
    }
    counter = 0;
    for (column = board->columns - 1; column >= 0; column--) {
        if (board->tilesBoard[0][column] == -1) {
            counter++;
        } else if (counter) {
            for (line = 0; line < board->lines; line++) {
                board->tilesBoard[line][column + counter] = board->tilesBoard[line][column];
                board->tilesBoard[line][column] = -1;
            }
        }
    }

    return;

}
