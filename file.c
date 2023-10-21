#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * readFile ()
 *
 * Arguments: FILE *file and int *error
 * Returns: Board *board
 * Side-Effects: changes the value of the second argument (error) if the problem is invalid
 *
 * Description: creates a Board and sets it up with the current problem information
 *****************************************************************************/

Board *getBoard(FILE *file, int *error){

    Board *board = (Board *) malloc (sizeof(Board));

    ushort numColor = 0;

    // getting the header of the problem
    if (fscanf(file, "%i %i %i",
        &board->lines, &board->columns, &board->variant) != 3) {
            free(board);
            return NULL;    
    } 
    
    // initializing the board matrix
    board->tiles = (Vector2 **) malloc (board->lines * sizeof(Vector2 *));

    for (uint k = 0; k < board->lines; k++) {
        board->tiles[k] = (Vector2 *) malloc (board->columns * sizeof(Vector2));
    }

    // getting every element off of the file
    for (int line = board->lines - 1, id = 0; line >= 0; line--){
        for (int column = 0; column < board->columns; column++){
            fscanf(file, "%i", &board->tiles[line][column].x);
            if (board->tiles[line][column].x > numColor) numColor = board->tiles[line][column].x;
            board->tiles[line][column].y = id;
            id++;
        }
    }

    board->colors = (uint *)malloc(numColor*sizeof(uint));
    board->numColors = numColor;
    for (; numColor > 0; numColor--) {
        board->colors[numColor-1] = 0;
    }

    /* Checking if the problem is invalid or not */
    if (board->variant != -1 && board->variant != -3 && board->variant < 0) {
        *error = 1;
    }
    /* Returns the board created */
    printf("got a new board!\n");
    return board;
}

/*char *outputName(char *inputName) {

    char *name;
    char *extension;
    uint len;

    len = strlen(inputName);

    name = (char *) malloc((len + 2) * sizeof(char));
    strcpy(name, inputName);

    extension = name + len - 10;
    memcpy(extension, ".toleblists", 11);

    return name;
}*/

char *outputName(char *inputName) {

    char *name;
    char extension[] = ".toleblists\0";
    uint len, i, j;
  
    len = strlen(inputName);

    name = (char *) malloc((len + 2) * sizeof(char));
    for (i = 0; i < len - 10; i++) {
        name[i] = inputName[i];
    }
    for (j = 0; i < len + 2; i++, j++) {
        name[i] = extension[j];
    }

    return name;
}

/******************************************************************************
 * writeFile ()
 *
 * Arguments: FILE *file, Board *board and int score
 * Returns: nothing
 * Side-Effects: changes the second argument (Board *) if it's the second variant
 *
 * Description: writes to the output file the answer to the problem according to its variant
 *****************************************************************************/

void writeFile(FILE *file, Board *board, Solution allMoves) {

    uint moves;
    VectorList *move;

    /* Writing the problem header */
    fprintf(file, "%i %i %i\n",
                board->lines, board->columns, board->variant);
    if (!allMoves.moves) {
        fprintf(file, "0 -1\n\n");
        return;
    } 
    for (moves = 0, move = allMoves.moves; move; move = move->next, moves++);
    fprintf(file, "%i %i\n", moves, allMoves.score);
    for (move = allMoves.moves; move; move = move->next) fprintf(file, "%i %i\n", move->tile.y, move->tile.x);
    fprintf(file, "\n");

    return;
}
