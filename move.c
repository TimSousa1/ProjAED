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

VectorList *addToVectorList(VectorList *head, Vector2 tile) {

    VectorList *vector = (VectorList *) malloc(sizeof(VectorList));

    vector->tile = tile;
    vector->next = head;

    return vector;
}

MoveList *revertMove(Board *board, MoveList* lastMove) {

    MoveList *ret;
    TileList *tmp1;
    VectorList *tmp2;

    while (lastMove->tileHead) {
        board->tiles[lastMove->tileHead->initPos.y][lastMove->tileHead->initPos.x].x = 
            board->tiles[lastMove->tileHead->finalPos.y][lastMove->tileHead->finalPos.x].x;
        board->tiles[lastMove->tileHead->finalPos.y][lastMove->tileHead->finalPos.x].x = -1;

        tmp1 = lastMove->tileHead;
        lastMove->tileHead = lastMove->tileHead->next;
        free(tmp1);
    }
    while (lastMove->removedTiles) {
        board->tiles[lastMove->removedTiles->tile.y][lastMove->removedTiles->tile.x].x = lastMove->color;
        board->colors[lastMove->color-1]++;
        tmp2 = lastMove->removedTiles;
        lastMove->removedTiles = lastMove->removedTiles->next;
        free(tmp2);
    }
    
    ret = lastMove->next;
    free(lastMove);
    return ret;
}

Solution createSolution(MoveList *moves) {
    Solution answer;
    answer.score = 0;
    answer.moves = NULL;
    if (!moves) return answer;
    VectorList *head = NULL;
    MoveList *aux;
    for (aux = moves; aux; aux = aux->next) {
        head = addToVectorList(head, aux->tile);
    } 
    answer.score = moves->score;
    answer.moves = head;
    return answer;
}

Solution solveVariant1(Board *board) {

    int id, numberOfTiles;
    uint tilesMoved;
    MoveList *head = NULL, *previous = NULL;
    Solution solution;

    while (1) {
        resetClusterSets(board);
        findAllClusters(board);
        id = findTopSweep(board);
        //printf("%i\n", id);
        if (id == -1) break;
        previous = head;
        //showBoard(board);
        //printf("--removing--\n\n");
        head = removeCluster(board, id);
        //printf("%i %i\n", head->tile.y, head->tile.x);
        //showBoard(board);
        //printf("--falling--\n\n");
        head->tileHead = applyGravity(board, &tilesMoved);
        if (previous) head->score += previous->score; 
        head->tilesMoved = tilesMoved;
        head->next = previous;
        //showBoard(board);
        //showMoveList(head);
        //showTileList(head->tileHead);
    }
    solution = createSolution(head);
    freeMoveList(head);
    return solution;
}

Solution solveVariant2or3(Board *board) {

    MoveList *head = NULL, *previous = NULL;
    Solution solution;
    int id, lastID = -1;
    int target = board->variant;
    uint tilesMoved;
    
    solution.score = 0;
    solution.moves = NULL;

    while (1) {
        resetClusterSets(board);
        findAllClusters(board);
        //printf("Making play: %i %i\n\n", play.y, play.x);
        id = idSweep(board, lastID);
        //printf("%i\n", id);
        if (id == -1 || hopeless(board, target, head)) {
            if (!head) {
                //printf("leaving\n");
                break;
            } 
            //printf("%i ", head->score);
            if (board->variant >= 0 && head->score > board->variant) {
                //printf("%p\n", head);
                solution = createSolution(head);
                freeMoveList(head);
                return solution;
            } if (board->variant == -3 && head->score > target) {
                target = head->score;
                //printf("New Target: %i\n", target);
                if (solution.moves) freeVectorList(solution.moves);
                solution = createSolution(head);
            }
            lastID = head->id;
            //printf("Next Play: %i %i\n\n", play.y, play.x);
            //showBoard(board);
            //printf("--reverting--\n\n");
            //showMoveList(head);
            head = revertMove(board, head);
            //showBoard(board);
            //showMoveList(head);
            continue;
        }
        previous = head;
        //showBoard(board);
        //printf("--removing--\n\n");
        //printf("%i\n", id);
        head = removeCluster(board, id);
        lastID = -1;
        //printf("%i %i\n", head->tile.y, head->tile.x);
        //showBoard(board);
        //printf("--falling--\n\n");
        head->tileHead = applyGravity(board, &tilesMoved);
        if (previous) head->score += previous->score; 
        head->tilesMoved = tilesMoved;
        head->next = previous;
        //showBoard(board);
        //showMoveList(head);
        //showTileList(head->tileHead);
    }
    showBoard(board);
    //printf("%p\n", solution);
    return solution;
}

void freeMoveList(MoveList *head) {

    MoveList *moveTmp;
    TileList *tileTmp;
    VectorList *vectorTmp;

    while (head) {
        while (head->tileHead) {
            tileTmp = head->tileHead;
            head->tileHead = head->tileHead->next;
            free(tileTmp);
        }
        while (head->removedTiles) {
            vectorTmp = head->removedTiles;
            head->removedTiles = head->removedTiles->next;
            free(vectorTmp);
        }
        moveTmp = head;
        head = head->next;
        free(moveTmp);
    }
    return;
}

void freeVectorList(VectorList *head) {

    VectorList *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }

}

void showMoveList(MoveList *lastMove) {
    printf("--------\n");
    for (MoveList *aux = lastMove; aux; aux=aux->next) {
        printf("%i %i\n", aux->tile.y, aux->tile.x);
    }
    printf("color: %i id: %i score: %i\n",
            lastMove->color, lastMove->id, lastMove->score);
    printf("--------\n\n");
}
