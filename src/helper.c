#include "../includes/variables.h"
#include <stdlib.h>

int checkWinner(int inputRow, int inputCol, int boardInput[3][3], int depth)
{
    //If roundsPlayed int is smaller than 5 there can be no winner yet
	if (roundsPlayed + depth < 5)
		return 0;

	// Check row
	if (boardInput[inputRow][0] == 1 && boardInput[inputRow][1] == 1 && boardInput[inputRow][2] == 1)
	   return -1;
	else if (boardInput[inputRow][0] == 2 && boardInput[inputRow][1] == 2 && boardInput[inputRow][2] == 2)
	   return 1;

    // Check column
	if (boardInput[0][inputCol] == 1 && boardInput[1][inputCol] == 1 && boardInput[2][inputCol] == 1)
	   return -1;
	else if (boardInput[0][inputCol] == 2 && boardInput[1][inputCol] == 2 && boardInput[2][inputCol] == 2)
	   return 1;

    // Check diagonal 1
	if (boardInput[0][2] == 1 && boardInput[1][1] == 1 && boardInput[2][0] == 1)
	   return -1;
   	else if (boardInput[0][2] == 2 && boardInput[1][1] == 2 && boardInput[2][0] == 2)
	   return 1;

    // Check diagonal 2
	if (boardInput[0][0] == 1 && boardInput[1][1] == 1 && boardInput[2][2] == 1)
	   return -1;
   	else if (boardInput[0][0] == 2 && boardInput[1][1] == 2 && boardInput[2][2] == 2)
	   return 1;

    return 0;
}

int *checkAvailableSpots(int boardInput[3][3])
{
    int *availableSpots = (int *)malloc(9 * sizeof(int));

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(boardInput[i][j] == 0){
                availableSpots[i * 3 + j] = 0;
            }
            else if(boardInput[i][j] == 1){
                availableSpots[i * 3 + j] = 1;
            }
            else if(boardInput[i][j] == 2){
                availableSpots[i * 3 + j] = 1;
            }
        }
    }
    return availableSpots;
}

int *numberToRowCol(int number)
{
    int column; //cleverly determine the columns place
    if(number % 3 == 0) {
        column = 2;
    }
    else{
        column = number % 3 - 1;
    }

    int row; // brute force the row place
    if(number < 4){
        row = 0;
    }
    else if (number < 7){
        row = 1;
    }
    else {
        row = 2;
    }

    int *rowCol = (int *)malloc(2 * sizeof(int));
    rowCol[0] = row;
    rowCol[1] = column;
    return rowCol;
}

int boardFull(int boardInput[3][3])
{
    int *posibilities = checkAvailableSpots(boardInput);
    int empty = 9;
    for(int i = 0; i < 9; i++){
        if(posibilities[i] != 0){
            empty--;
        }
    }
    free(posibilities);
    return empty;
}