#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>


CellList *createCell(Board *board, int line, int column, uint id, int color);
void listAdd(CellList *element, CellList *toAdd);
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

CellList *findCluster(Board *board, int line, int column){

    /* Remember the id of the cluster */
    uint headID = board->clusterSets[(line - 1) * board->columns + column - 1];
    
    /* Allocating memory for the CellList head */
    CellList *head = (CellList*) malloc (sizeof(CellList));

    CellList *current = NULL;

    /* Filling the with correct information in the board */
    head->line = line - 1;
    head->column = column - 1;
    head->color = board->tiles[line - 1][column - 1];
    head->next = NULL;

    if (head->color == -1) return head;

    for (current = head; current;){

        /* This adds adjacent tiles that have the same color to the list in order to check the tiles adjacent to them later on */
        listAdd(current, createCell(board, current->line + 1, current->column, headID, head->color)); // up
        listAdd(current, createCell(board, current->line - 1, current->column, headID, head->color)); // down
        listAdd(current, createCell(board, current->line, current->column - 1, headID, head->color)); // left
        listAdd(current, createCell(board, current->line, current->column + 1, headID, head->color)); // right

        /* Check the next tile for all neighbours of the same color */
        current = current->next;
    }

    return head;
}

/******************************************************************************
 * createCell ()
 *
 * Arguments: Board *board, int line, int column, uint id, int color
 * Returns: CellList *cell
 * Side-Effects: Creates a CellList *
 *
 * Description: Creates a Cell if its tile has the same color as the original tile
 *****************************************************************************/

CellList *createCell(Board *board, int line, int column, uint id, int color){
    
    /* Checking if the we're looking for a tile outside of the matrix */
    if (line > board->lines - 1 || line < 0 ||
            column > board->columns - 1 || column < 0){
        return NULL;
    }
    /* Checking if the tile has the same color */
    if (board->tiles[line][column] != color) return NULL;

    uint i = line * board->columns + column;

    /* Checking if it has already been added */
    if (board->clusterSets[i] == id) return NULL;
    board->clusterSets[i] = id;

    /* Allocating memory for the new cell */
    CellList *cell = (CellList*) malloc (sizeof(CellList));

    /* Filling the cell with the correct information */
    cell->line = line;
    cell->column = column;
    cell->color = board->tiles[line][column];
    cell->next = NULL;

    /* Returning the created cell */
    return cell;
}

/******************************************************************************
 * listAdd ()
 *
 * Arguments: CellList *element and CellList *toAdd
 * Returns: nothing
 * Side-Effects: Puts the second argument after ithe first in its list
 *
 * Description: Adds a new element to a list 
 * *****************************************************************************/

void listAdd(CellList *element, CellList *toAdd){
    
    /* Checking if we are trying to add nothing */
    if (!toAdd) return;
    
    /* Adding the cells in order */
    toAdd->next = element->next;
    element->next = toAdd;

    return;
}


/******************************************************************************
 * removeCluster ()
 *
 * Arguments: Board *board and CellList *head
 * Returns: nothing
 * Side-Effects: Changes the board->tiles by removing a cluster 
 * 
 * Description: Removes a cluster from a board->tiles
 * *****************************************************************************/

void removeCluster(Board *board, CellList *head) {

    CellList *aux;

    /* Making sure the cluster isn't a single tile */
    if (!head->next) return;

    /* Removing each member of the cluster by changing its color to -1 */
    for (aux = head; aux; aux = aux->next) {
        board->tiles[aux->line][aux->column] = -1;
    }

    /* Making sure to apply gravity after removing a cluster */
    applyGravity(board);

    return;

}

/******************************************************************************
 * applyGravity ()
 *
 * Arguments: Board *board
 * Returns: nothing
 * Side-Effects: Changes the board->tiles as if gravity took effect to the tiles
 
 * Description: Simulates vertical and horizontal gravity 
 * *****************************************************************************/

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

/******************************************************************************
 * getScore ()
 *
 * Arguments: CellList *head
 * Returns: uint score
 * Side-Effects: none
 *
 * Description: Calculates the score of a cluster
 * *****************************************************************************/

uint getScore(CellList *head) {

    uint numOfBlocks = 0;
    CellList *aux; 

    /* Counts the number of tiles in the cluster */
    for (aux = head; aux; aux = aux->next) numOfBlocks++;

    return numOfBlocks * (numOfBlocks - 1);
}

/******************************************************************************
 * freeBoard ()
 * Arguments: Board *board
 * Returns: nothing
 * Side-Effects: Frees its argument
 *
 * Description: Frees the memory allocated for a Board
 * *****************************************************************************/

void freeBoard(Board *board) {

    int i;
    /* Freeing the memory allocated for the tiles matrix */
    for (i = 0; i < board->lines; i++) {
        free(board->tiles[i]);
    }
    free(board->tiles);
    
    /* Freeing the memory allocated for the clustersets array */
    free(board->clusterSets);

    /* Freeing the memory allocated for the board */
    free(board);

    return;
    
}

/******************************************************************************
 * freeCluster ()
 *
 * Arguments: CellList *head
 * Returns: nothing
 * Side-Effects: Frees its argument
 *
 * Description: Frees the memory allocated for a CellList
 * *****************************************************************************/

void freeCluster(CellList *head) {

    CellList *tmp;

    /* Freeing the memory allocated for each member of the list */
    while (head) {
        tmp = head->next;
        free(head);
        head = tmp;
    }

    return;
}
