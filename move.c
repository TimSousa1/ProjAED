#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

TileList *addToTileList(TileList *head, Vector2 initPos, Vector2 finalPos) {

    TileList *tile = (TileList *) malloc(sizeof(TileList));

    tile->initPos = initPos;
    tile->finalPos = finalPos;
    tile->next = head;

    return tile;
}

MoveList *createMoveList() {
    MoveList *move = (MoveList *) malloc(sizeof(MoveList));
    return move;
}

TileList *createTileList() {
    TileList *tile = (TileList *) malloc(sizeof(TileList));
    return tile;
}

MoveList *revertMove(Board *board, MoveList* lastMove) {

    TileList *currentTile, *tmp;
    MoveList *ret;
    uint line, column;

    for (currentTile = lastMove->tileHead; currentTile;) {
        board->tiles[currentTile->initPos.y][currentTile->initPos.x].x = board->tiles[currentTile->finalPos.y][currentTile->finalPos.x].x;
        board->tiles[currentTile->finalPos.y][currentTile->finalPos.x].x = lastMove->color;
        tmp = currentTile;
        currentTile = currentTile->next;
        free(tmp);
        //showBoard(board);
    }
    for (uint line = 0; line < board->lines; line++) {
        for (uint column = 0; column < board->columns; column++) {
            if (board->tiles[line][column].x == -1) board->tiles[line][column].x = lastMove->color;
            else if (board->tiles[line][column].x < -1) board->tiles[line][column].x++;
        }
    }
    ret = lastMove->next;
    free(lastMove);
    return ret;
}

MoveList *solveVariant1(Board *board) {

    int id, numberOfTiles;
    uint tilesMoved;
    MoveList *moves;

    while (1) {
            //showboard(board);
            //showID(board);

            id = findTopSweep(board);
            if (id == -1) break;

            moves = removeCluster(board, id);
            resetClusterSets(board);

            applyGravity(board, &tilesMoved);
            board->score += numberOfTiles * (numberOfTiles - 1);
            showBoard(board);
            printf("score: %i\n", board->score);
            showID(board);
    }

    return moves;
}

MoveList *createSolution(MoveList *moves) {

    MoveList *aux, *newMove, *previous = NULL, *head = NULL;

    for (aux = moves; aux; aux = aux->next) {
        newMove = createMoveList();
        newMove->tile = aux->tile;
        newMove->score = aux->score;
        newMove->next = NULL;
        if (!previous) head = newMove;
        if (previous) previous->next = newMove;
        previous = aux;
    }
    return head;
}

MoveList *solveVariant2or3(Board *board) {

    MoveList *head = NULL, *previous = NULL, *solution = NULL;
    Vector2 lastPlay;
    int id;
    uint canEnd = 0, target = board->variant, tilesMoved;
    
    lastPlay.x = -1;
    lastPlay.y = -1;

    while (1) {
        resetClusterSets(board);
        findAllClusters(board);
        id = findBottomSweep(board, lastPlay);
        if (id == -1 || hopeless(board, target)) {
            if (!head) {
                printf("leaving\n");
                break;
            } if (board->variant >= 0 && head->score > board->variant) {
                printf("%p\n", head);
                return head;
            } 
            canEnd = 1;
            if (board->variant == -3 && head->score > target) {
                target = head->score;
                if (solution) freeMoveList(solution);
                solution = createSolution(head);
            }
            lastPlay = head->tile;
            printf("%i %i\n\n", lastPlay.y, lastPlay.x);
            showBoard(board);
            printf("--reverting--\n\n");
            showMoveList(head);
            head = revertMove(board, head);
            showBoard(board);
            showMoveList(head);
            continue;
        }
        showBoard(board);
        printf("--removing--\n\n");
        head = removeCluster(board, id);
        lastPlay.x = -1;
        lastPlay.y = -1;
        printf("%i %i\n", head->tile.y, head->tile.x);
        showBoard(board);
        printf("--falling--\n\n");
        head->tileHead = applyGravity(board, &tilesMoved);
        head->tilesMoved = tilesMoved;
        head->next = head;
        head = head;
        showBoard(board);
        showMoveList(head);
        showTileList(head->tileHead);
    }
    //showBoard(board);
    printf("%p\n", solution);
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

void showMoveList(MoveList *lastMove) {

    for (MoveList *aux; aux; aux=aux->next) {
        printf("%i %i\n", aux->tile.y, aux->tile.x);
    }

}