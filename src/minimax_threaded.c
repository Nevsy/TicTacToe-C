#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../includes/variables.h"
#include "../includes/helper.h"

typedef struct minimaxArgs
{
    int move[2];
    int boardInput[3][3];
    int depth;
    int player;
    int score;
} minimaxArgs;

int miniMax(int *move, int boardInput[3][3], int depth, int player);

void* threadedMinimax(void* arg) {
    minimaxArgs* args = (minimaxArgs*)arg;
    args->score = miniMax(args->move, args->boardInput, args->depth, args->player);
    return NULL;
}

int *miniMaxInitCall(int boardInput[3][3], int player)
{
    int *possibilities = checkAvailableSpots(boardInput);
    int *bestMove = malloc(2 * sizeof(int));
    pthread_t threads[9];
    minimaxArgs args[9];
    
    int oppositePlayer = (player == 1) ? 2 : 1;
    int bestScore = (player == 1) ? -2 : 2; // Initialize best score based on player

    for (int i = 0; i < 9; i++) {
        if (possibilities[i] == 0) {
            int *move = numberToRowCol(i + 1);
            memcpy(args[i].move, move, 2 * sizeof(int));
            memcpy(args[i].boardInput, boardInput, 9 * sizeof(int));
            args[i].depth = 1;
            args[i].player = oppositePlayer;
            
            pthread_create(&threads[i], NULL, threadedMinimax, (void*)&args[i]);
            
            free(move);
        }
    }

    for (int i = 0; i < 9; i++) {
        if (possibilities[i] == 0) {
            pthread_join(threads[i], NULL);
            
            if ((player == 1 && args[i].score > bestScore) ||
                (player == 2 && args[i].score < bestScore)) {
                bestScore = args[i].score;
                memcpy(bestMove, args[i].move, 2 * sizeof(int));
            }
        }
    }

    // Update the board and print the move
    if (roundsPlayed % 2 == 1)
        board[bestMove[0]][bestMove[1]] = firstPlayer + 1;
	else
        board[bestMove[0]][bestMove[1]] = firstPlayer ? 1 : 2;
    printf("The computer played on row %d and column %d\n", bestMove[0] + 1, bestMove[1] + 1);

    free(possibilities);
    return bestMove;
}

int miniMax(int *move, int boardInput[3][3], int depth, int player)
{
    int boardCopy[3][3] = {{0}}; // Initialize all elements to zero
    for (int i=0; i<3; i++){
        for (int j = 0; j < 3; j++)
            boardCopy[i][j] = boardInput[i][j];
    }

    boardCopy[move[0]][move[1]] = player;
    int positionEval = checkWinner(move[0], move[1], boardCopy, depth);
    if (positionEval != 0)
        return positionEval;

    int numberOfPossibilities = boardFull(boardCopy);

    if(numberOfPossibilities == 0){ // Board is full
        return 0;
    }

    int *possibilities = checkAvailableSpots(boardCopy);

    if(player == 1){
        int best = -2; // lower than lowest possible score
        for(int i = 0; i < 9; i++){
            if(possibilities[i] == 0){
                int *move2 = numberToRowCol(i + 1);
                int score = miniMax(move2, boardCopy, depth + 1, 2);
                if(score > best) // maximizing player
                    best = score;
				free(move2);
            }
        }
        free(possibilities);
        return best;
    }
    else if(player == 2){
        int best = 2; // higher than highest possible score
        for(int i = 0; i < 9; i++){
            if (possibilities[i] == 0){
                int *move2 = numberToRowCol(i + 1);
                int score = miniMax(move2, boardCopy, depth + 1, 1);
                if (score < best) // minimizing player
                    best = score;
				free(move2);
            }
        }
        free(possibilities);
        return best;
    }
	else
	{
		printf("player is not 1 neither 2");
		exit(1);
	}
}