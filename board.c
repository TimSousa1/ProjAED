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
void listRemove(CellList *previous);
void showCell(CellList *cell);
void showCells(CellList *head);
void showID(ushort *id, int size);
void applyGravity(Board *board); 

void findTileCluster(Board *board, short line, short column){
    ushort headID = board->clusterSets[(line - 1) * board->columns + column - 1];
    //printf("creating list with headID %i\n", headID);
    CellList *head = (CellList*) malloc (sizeof(CellList));
    CellList *current = NULL;
    CellList *previous = NULL;

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
        previous = current;
        current = current->next;
        //showCells(head);
    }
    //showID(board->clusterSets, board->lines * board->columns);

    return;
}

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

void listAdd(CellList *element, CellList *toAdd){
    if (!toAdd) return;
     
    toAdd->next = element->next;
    element->next = toAdd;
}

// remove the next element to the one being passed 
void listRemove(CellList *previous){
    CellList *tmp = previous->next;
    previous->next = previous->next->next;
    free(tmp);
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

void removeCluster(Board *board) {

    short cluster, i, j, k;

    cluster = board->clusterSets[board->l * board->columns + board->c];

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

void applyGravity(Board *board) {

    short i, j, slide;

    for (i = 0; i < board->lines - 1; i++) {
        for (j = 0; j < board->columns; j++) {
            if (board->tilesBoard[i][j] == -1) {
                board->tilesBoard[i][j] = board->tilesBoard[i + 1][j];
                board->tilesBoard[i + 1][j] = -1;
            }
        }
    }
    for (j = board->columns - 1; j > 0; j--) {
        slide = 1;
        for (i = 0; i < board->lines; i++) {
            if (board->tilesBoard[i][j] == -1) slide = 0;
        }
        if (slide == 1) {
            for (i = 0; i < board->lines; i++) {
                board->tilesBoard[i][j] = board->tilesBoard[i][j - 1];
                board->tilesBoard[i][j - 1] = -1;
            }
        }
    }

    return;

}
