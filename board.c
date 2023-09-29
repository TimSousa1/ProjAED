#include "read.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _cellList{
    int x;
    int y;
    int color;

    struct _cellList *next;
} CellList;

CellList *createCell(Board *board, int line, int color);
void listAdd(CellList *element, CellList *toAdd);

short findTileCluster(Board *board, short line, short column){
    short score;
    CellList *head = (CellList*) malloc (sizeof(CellList));
    CellList *current = NULL;

    head->x = line;
    head->y = column;
    head->color = board->tilesBoard[line - 1][column - 1];
    head->next = NULL;

    // TODO: check element's color
    for (current = head; current->next;){

        listAdd(current, createCell(board, line - 1 + 1, column - 1)); // up
        listAdd(current, createCell(board, line - 1 - 1, column - 1)); // down
        listAdd(current, createCell(board, line - 1, column - 1 - 1)); // left
        listAdd(current, createCell(board, line - 1, column - 1 + 1)); // right

        // should only run if for condition is met
        current = current->next;
    }

    score = current->color;
    return score;
}

CellList *createCell(Board *board, int line, int color){
    CellList *cell = (CellList*) malloc (sizeof(CellList));

    cell->x = line;
    cell->y = color;
    cell->color = board->tilesBoard[line][color];

    return cell;
}

void listAdd(CellList *element, CellList *toAdd){
    toAdd->next = element->next;
    element->next = toAdd;
}

void showBoard(Board *board){

    for (short i = board->lines - 1; i >= 0; i--){
        for (short j = 0; j < board->columns; j++){
            printf("%hi ", board->tilesBoard[i][j]);
        }
        printf("\n");
    }
}
