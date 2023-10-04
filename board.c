#include "common.h"
#include <stdio.h>
#include <stdlib.h>


CellList *createCell(Board *board, int line, int column, ushort id, short color);
void listAdd(CellList *element, CellList *toAdd);
void applyGravity(Board *board);

/******************************************************************************
 * findTileCluster ()
 *
 * Arguments: Board *, short and short
 * Returns: CellList *
 * Side-Effects: Changes the clusterSets of the first argument (Board *)
 *
 * Description: Groups the tile provided as an argument in a cluster
 *****************************************************************************/

CellList *findTileCluster(Board *board, short line, short column){
    ushort headID = board->clusterSets[(line - 1) * board->columns + column - 1];
    CellList *head = (CellList*) malloc (sizeof(CellList));
    CellList *current = NULL;

    head->line = line - 1;
    head->column = column - 1;
    head->color = board->tilesBoard[line - 1][column - 1];
    head->next = NULL;

    for (current = head; current;){

        /* This adds adjacent tiles that have the same color to the list in order to check the tiles adjacent to them later on */
        listAdd(current, createCell(board, current->line + 1, current->column, headID, head->color)); // up
        listAdd(current, createCell(board, current->line - 1, current->column, headID, head->color)); // down
        listAdd(current, createCell(board, current->line, current->column - 1, headID, head->color)); // left
        listAdd(current, createCell(board, current->line, current->column + 1, headID, head->color)); // right

        current = current->next;
    }

    return head;
}

/******************************************************************************
 * createCell ()
 *
 * Arguments: Board *, int, int, ushort, short
 * Returns: CellList *
 * Side-Effects: Creates a CCircuitosellList *
 *
 * Description: Creates a Cell if its tile has the same color as the original tile
 *****************************************************************************/

CellList *createCell(Board *board, int line, int column, ushort id, short color){
    
    if (line > board->lines - 1 || line < 0 ||
            column > board->columns - 1 || column < 0){
        return NULL;
    }
    if (board->tilesBoard[line][column] != color) return NULL;

    ushort i = line * board->columns + column;

    if (board->clusterSets[i] == id) return NULL;
    board->clusterSets[i] = id;

    CellList *cell = (CellList*) malloc (sizeof(CellList));

    cell->line = line;
    cell->column = column;
    cell->color = board->tilesBoard[line][column];
    cell->next = NULL;

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

void freeCluster(CellList *head) {

    CellList *tmp;

    while (head) {
        tmp = head->next;
        free(head);
        head = tmp;
    }

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

void removeCluster(Board *board, CellList *head) {

    CellList *aux;

    if (!head->next) return;

    for (aux = head; aux; aux = aux->next) {
        board->tilesBoard[aux->line][aux->column] = -1;
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

ushort getScore(Board *board, CellList *head) {

    ushort numOfBlocks = 0;
    CellList *aux; 

    for (aux = head; aux; aux = aux->next) if (board->tilesBoard[aux->line][aux->column] != -1) numOfBlocks++;

    return numOfBlocks * (numOfBlocks - 1);
}

