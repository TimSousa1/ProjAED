#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

void addToTileList(TileList *head, Vector2 initPos, Vector2 finalPos) {

    TileList *tile = (TileList *) malloc(sizeof(TileList));

    tile->initPos = initPos;
    tile->finalPos = finalPos;
    tile->next = NULL;

    if (!head) {
        head = tile;
    } else {
        tile->next = head->next;
        head->next = tile;
    }
}

void revertMove(Board *board, MoveList* lastMove) {

    tileList *currentTile, *tmp;

    for (currentTile = lastMove->tileHead; currentTile; currentTile = currentTile->next) {
        board->tiles[currentTile->initPos.y][currentTile->initPos.x].x = board->tiles[currentTile->finalPos.y][currentTile->finalPos.x].x;
        board->tiles[currentTile->finalPos.y][currentTile->finalPos.x].x = lastMove->color;
    }
    for (currentTile = lastMove->tileHead; currentTile;) {
        tmp = currentTile;
        currentTile = currentTile->next;
        free(tmp);
    }
    lastMove->previous->next = NULL;
    free(lastMove);
}

MoveList *solveVariant1(Board *board) {

    int id, numberOfTiles; 

    while (1) {
            //showboard(board);
            //showID(board);

            id = findTopSweep(board);
            if (id == -1) break;

            numberOfTiles = removeCluster(board, id);
            resetClusterSets(board);

            applyGravity(board);
            moveHead = moveListAdd(moveHead, line, column);
            board->score += numberOfTiles * (numberOfTiles - 1);
            showBoard(board);
            printf("score: %i\n", board->score);
            showID(board);
    }

    return moveHead;
}

MoveList *solveVariant2or3(Board *board) {

    MoveList *head, *currentMove, *previous = NULL, *solution;
    int a = 2;
    while (a) {
        a--;
        findAllClusters(board);
        id = findTopSweep(board);
        /*if (id == -1) {
            revertMove(board, currentMove);
            continue;
        }*/
        currentMove = removeCluster(board, id);
        currentMove->tileHead = applyGravity(board);
        currentMove->previous = previous;
        if (previous) previous->next = currentMove;
        previous = currentMove;
    }
    revertMove(board, previous);
    return solution;
}