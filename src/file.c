#include "tileblaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * getBoard()
 *
 * Arguments: FILE *file and int *error
 * Returns: Board *board
 * Side-Effects: changes the value of the second argument (error) if the problem is invalid
 *
 * Description: creates a Board and sets it up with the current problem information
 *****************************************************************************/

Board *getBoard(FILE *file, int *error){

    Board *board = (Board *) malloc (sizeof(Board));
    if (!board) {
        fprintf(stderr, "Error allocating memory for variable \"board\", in function getBoard()!\n");
        exit(1);
    }
    ushort numColor = 0;

    // getting the header of the problem
    if (fscanf(file, "%i %i %i",
        &board->lines, &board->columns, &board->variant) != 3) {
            free(board);
            return NULL;    
    } 
    
    // initializing the board matrix
    board->tiles = (Vector2 **) malloc (board->lines * sizeof(Vector2 *));

    if (!board->tiles) {
        fprintf(stderr, "Error allocating memory for variable \"board->tiles\", in function getBoard()!\n");
        exit(1);
    }

    for (int k = 0; k < board->lines; k++) {
        board->tiles[k] = (Vector2 *) malloc (board->columns * sizeof(Vector2));
        if (!board->tiles[k]) {
            fprintf(stderr, "Error allocating memory for variable \"board->tiles[%i]\", in function getBoard()!\n", k);
            exit(1);
        }
    }

    // getting every element off of the file
    for (int line = board->lines - 1; line >= 0; line--){
        for (int column = 0; column < board->columns; column++){
            if (fscanf(file, "%i", &board->tiles[line][column].x) != 1) *error = 1;
            if (board->tiles[line][column].x > numColor) numColor = board->tiles[line][column].x;
        }
    }
    resetClusterSets(board);

    board->colors = (uint *)calloc(numColor, sizeof(uint));
    if (!board->colors) {
        fprintf(stderr, "Error allocating memory for variable \"board->colors\", in function getBoard()!\n");
        exit(1);
    }
    board->numColors = numColor;

    /* Checking if the problem is invalid or not */
    if (board->variant != -1 && board->variant != -3 && board->variant < 0) {
        *error = 1;
    }
    /* Returns the board created */
    return board;
}

/******************************************************************************
 * outputName()
 *
 * Arguments: char *inputName
 * Returns: char *outputName
 * Side-Effects: none
 *
 * Description: creates the name for the output file. Returns NULL if the input name is invalid
 *****************************************************************************/

char *outputName(char *inputName) {

    char *outputName, *checkName;
    char inputExtension[] = ".tilewalls";
    char outputExtension[] = ".tileblasts";
    uint len, i, j;
  
    len = strlen(inputName);

    checkName = inputName + len - strlen(inputExtension);
    if (strcmp(checkName, inputExtension) != 0) return NULL;

    outputName = (char *) malloc((len + 2) * sizeof(char));
    if (!outputName) {
        fprintf(stderr, "Error allocating memory for varaible \"outputName\", in function outputName()!\n");
        exit(1);
    }

    for (i = 0; i < len - 10; i++) {
        outputName[i] = inputName[i];
    }
    for (j = 0; i < len + 2; i++, j++) {
        outputName[i] = outputExtension[j];
    }

    return outputName;
}

/******************************************************************************
 * writeFile()
 *
 * Arguments: FILE *file, Vector2 boardSize, int variant and Solution answer
 * Returns: nothing
 * Side-Effects: none
 *
 * Description: writes to the output file the answer to the problem
 *****************************************************************************/

void writeFile(FILE *file, Vector2 boardSize, int variant, Solution answer) {

    uint moves;
    VectorList *move;

    /* Writing the problem header */
    fprintf(file, "%i %i %i\n",
                boardSize.y, boardSize.x, variant);
    if (!answer.moves) {
        if (variant > 0) {
            fprintf(file, "0 -1\n\n");
        } else {
            fprintf(file, "0 0\n\n");
        }
        return;
    } 
    for (moves = 0, move = answer.moves; move; move = move->next, moves++);
    fprintf(file, "%i %i\n", moves, answer.score);
    for (move = answer.moves; move; move = move->next) fprintf(file, "%i %i\n", move->tile.y, move->tile.x);
    fprintf(file, "\n");

    return;
}
