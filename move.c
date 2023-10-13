#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>

MoveList *moveListAdd(MoveList *head, uint line, uint column) {
    MoveList *move = (MoveList *) malloc(sizeof(MoveList));
    MoveList *last;

    move->line = line;
    move->column = column;
    move->next = NULL;
    if (!head) return move;
    for (last = head; last->next != NULL; last = last->next);
    last->next = move;
    return head;
}

void freeMoveList(MoveList *head) {
    MoveList *tmp;
    for (tmp = head; tmp; free(head)) {
        head = tmp;
        tmp = tmp->next;
    }
}