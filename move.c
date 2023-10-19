#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

TileList *addToTileList(TileList *head, Vector2 initPos, Vector2 finalPos) {

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
    return head;
}

void revertMove(Board *board, MoveList* lastMove) {

    TileList *currentTile, *tmp;
    uint line, column;

    for (currentTile = lastMove->tileHead; currentTile;) {
        board->tiles[currentTile->initPos.y][currentTile->initPos.x].x = board->tiles[currentTile->finalPos.y][currentTile->finalPos.x].x;
        board->tiles[currentTile->finalPos.y][currentTile->finalPos.x].x = -1;
        tmp = currentTile;
        currentTile = currentTile->next;
        free(tmp);
    }
    for (uint line = 0; line < board->lines; line++) {
        for (uint column = 0; column < board->columns; column++) {
            if (board->tiles[line][column].x == -1) board->tiles[line][column].x = lastMove->color;
            else if (board->tiles[line][column].x < -1) board->tiles[line][column].x++;
        }
    }
    if (lastMove->previous) lastMove->previous->next = NULL;
    free(lastMove);
}

MoveList *solveVariant1(Board *board) {

    int id, numberOfTiles;
    MoveList *moves;

    while (1) {
            //showboard(board);
            //showID(board);

            id = findTopSweep(board);
            if (id == -1) break;

            moves = removeCluster(board, id);
            resetClusterSets(board);

            applyGravity(board);
            board->score += numberOfTiles * (numberOfTiles - 1);
            showBoard(board);
            printf("score: %i\n", board->score);
            showID(board);
    }

    return moves;
}

MoveList *solveVariant2or3(Board *board) {

    MoveList *head, *currentMove = NULL, *previous = NULL, *solution;
    int a = 1, id;
    while (a) {
        a--;
        previous = currentMove;
        findAllClusters(board);
        id = findBottomSweep(board);
        /*if (id == -1) {
            revertMove(board, currentMove);
            continue;
        }*/
        currentMove = removeCluster(board, id);
        currentMove->tileHead = applyGravity(board);
        currentMove->previous = previous;
        if (previous) previous->next = currentMove;
        showBoard(board);
        showTileList(currentMove->tileHead);
    }
    revertMove(board, currentMove);
    showBoard(board);
    return solution;
}

void freeMoveList(MoveList *head) {

    MoveList *moveTmp;
    TileList *tileTmp;

    while (head) {
        while (head->tileHead) {
            tileTmp = head->tileHead;
            head->tileHead = head->tileHead->next;
            free(tileTmp);
        }
        moveTmp = head;
        head = head->next;
        free(moveTmp);
    }
    return;
}