#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

VectorList *addToVectorList(VectorList *head, Vector2 tile) {

    VectorList *vector = (VectorList *) malloc(sizeof(VectorList));

    vector->tile = tile;
    vector->next = head;

    return vector;
}

// returns the new head of a stack VectorList
VectorList *removeVector(VectorList *head) {

    VectorList *ret = head->next;
    free(head);
    return ret;

}

MoveList *revertMove(MoveList* lastMove) {
    MoveList *current = lastMove->next, *moveTmp;

    VectorList *tmp;
    while (lastMove->clusters) {
        tmp = lastMove->clusters;
        lastMove->clusters = lastMove->clusters->next;
        free(tmp);
    }
    
    freeBoard(lastMove->board);
    free(lastMove);
    return current;
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
    origin->next = NULL;
    origin->board = board;

    resetClusterSets(origin->board);
    origin->clusters = findAllClusters(origin->board);

    current = origin;

    while (1) {

        //showVectorList(current->clusters);
        if (!current->clusters || hopeless(current->board, target, current)) {
            //printf("no clusters found!\n");
            if (origin == current) {
                //printf("leaving..\n");
                break;
            } 
            //printf("%i ", head->score);
            if (current->board->variant >= -1 && current->score > current->board->variant) {
                solution = createSolution(current);
                freeMoveList(current);
                return solution;
            }
            if (current->board->variant == -3 && current->score > target) {
                target = current->score;
                //printf("New Target: %i\n", target);
                if (solution.moves) freeVectorList(solution.moves);
                solution = createSolution(current);
            }
            //printf("Next Play: %i %i\n\n", play.y, play.x);
            //showBoard(board);
            //printf("--reverting--\n\n");
            //showMoveList(head);

            current = revertMove(current);

            //showBoard(board);
            //showMoveList(current);
            continue;
        }
        previous = current;

        //showBoard(board);
        //printf("--removing--\n\n");
        //printf("%i\n", id);
    
        current = removeCluster(current->board,  current->clusters->tile);

        //printf("%i %printf("empty tile! moving on..\n");i\n", head->tile.y, head->tile.x);
        //showBoard(board);
        //printf("--falling--\n\n");

        //showBoard(board);
        if (previous) {
            current->score += previous->score;
            previous->clusters = removeVector(previous->clusters);
        }
        current->next = previous;

        //showBoard(board);
        //showMoveList(current);
        showVectorList(current->clusters);
        //showTileList(head->tileHead);
    }
    //showBoard(board);
    free(origin);
    //printf("%p\n", solution);
    return solution;
}

void freeMoveList(MoveList *head) {

    MoveList *moveTmp;
    TileList *tileTmp;
    VectorList *vectorTmp;

    while (head) {
        while (head->clusters) {
            vectorTmp = head->clusters;
            head->clusters = head->clusters->next;
            free(vectorTmp);
        }
        freeBoard(head->board);
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

void showMoveList(MoveList *move) {
    printf("--------\n");
    for (MoveList *aux = move; aux; aux=aux->next) {
        printf("%i %i\n", aux->tile.y, aux->tile.x);
    }
    printf("color: %i id: %i score: %i\n",
            move->color, move->id, move->score);
    printf("--------\n\n");
    showBoard(move->board);
}
