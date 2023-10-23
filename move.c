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

VectorList *removeVector(VectorList *head) {

    VectorList *ret = head->next;
    free(head);
    return ret;

}

MoveList *revertMove(Board *board, MoveList* lastMove) {

    MoveList *ret;
    TileList *tmp1;
    VectorList *tmp2;

    while (lastMove->tileHead) {
        board->tiles[lastMove->tileHead->initPos.y-1][lastMove->tileHead->initPos.x-1].x = 
            board->tiles[lastMove->tileHead->finalPos.y-1][lastMove->tileHead->finalPos.x-1].x;
        board->tiles[lastMove->tileHead->finalPos.y-1][lastMove->tileHead->finalPos.x-1].x = -1;

        tmp1 = lastMove->tileHead;
        lastMove->tileHead = lastMove->tileHead->next;
        free(tmp1);
    }
    while (lastMove->removedTiles) {
        board->tiles[lastMove->removedTiles->tile.y-1][lastMove->removedTiles->tile.x-1].x = lastMove->color;
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
    for (aux = moves; aux->next; aux = aux->next) {
        head = addToVectorList(head, aux->tile);
    } 
    answer.score = moves->score;
    answer.moves = head;
    return answer;
}

#if 1
#else
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
        
        head = removeCluster(board, current->clusters->tile);

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
#endif

Solution solve(Board *board) {

    MoveList *origin, *previous = NULL, *current;
    int id;
    int target = board->variant;

    Solution solution;
    VectorList *possibleMoves;
    
    solution.score = 0;
    solution.moves = NULL;

    origin = (MoveList *) malloc(sizeof(*origin));

    origin->tile.x = -1;
    origin->tile.y = -1;
    origin->score = 0;
    origin->id = -1;
    origin->color = -1;
    origin->clusters = NULL;
    origin->removedTiles = NULL;
    origin->tileHead = NULL;
    origin->next = NULL;

    current = origin;

    while (1) {
        if (!current->clusters) {
            resetClusterSets(board);
            current->clusters = findAllClusters(board);
        } else {
            current->clusters = removeVector(current->clusters);
        }
        showVectorList(current->clusters);
        if (!current->clusters || hopeless(board, target, current)) {
            if (origin == current) {
                //printf("leaving\n");
                break;
            } 
            //printf("%i ", head->score);
            if (board->variant >= -1 && current->score > board->variant) {
                //printf("%p\n", head);
                solution = createSolution(current);
                freeMoveList(current);
                return solution;
            } if (board->variant == -3 && current->score > target) {
                target = current->score;
                //printf("New Target: %i\n", target);
                if (solution.moves) freeVectorList(solution.moves);
                solution = createSolution(current);
            }
            //printf("Next Play: %i %i\n\n", play.y, play.x);
            showBoard(board);
            printf("--reverting--\n\n");
            //showMoveList(head);
            
            current = revertMove(board, current);
            
            showBoard(board);
            showMoveList(current);
            continue;
        }
        previous = current;

        showBoard(board);
        printf("--removing--\n\n");
        //printf("%i\n", id);
        
        current = removeCluster(board, current->clusters->tile);

        //printf("%i %i\n", head->tile.y, head->tile.x);
        showBoard(board);
        //printf("--falling--\n\n");
        
        current->tileHead = applyGravity(board);
        showBoard(board);
        if (previous) current->score += previous->score; 
        current->next = previous;

        showBoard(board);
        showMoveList(current);
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

    if (!head) return;
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
