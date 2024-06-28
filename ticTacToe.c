#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

void printBoard();
void resetBoard();

int checkWinner(int inputRow, int inputCol, int boardInput[3][3], int depth);
void checkGameConditions(int row, int column, int boardInput[3][3], int depth);
int *numberToRowCol(int number);
int boardFull(int boardInput[3][3]);
int *checkAvailableSpots(int boardInput[3][3]);

//int *computerPlaysRandomly();
int *miniMaxInitCall(int boardInput[3][3], int player);
int miniMax(int *move, int boardInput[3][3], int depth, int player);

int board[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}};
int determinedWinner = 0;
int firstPlayer;
char opponent[15];
int roundsPlayed = 0;

int main(int argc, char *argv[]) {
    // random time based on nano seconds
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);

    resetBoard();

    if(argc == 2 || argc == 3) {
        if(!strcasecmp(argv[1], "otb")){
            strcpy(opponent, "Player 2");
        }
        else if(!strcasecmp(argv[1], "computer")){
            strcpy(opponent, "alphaTicTac");
        }
        else if(!strcasecmp(argv[1], "-h") || !strcasecmp(argv[1], "--help")){
            printf("Choose who you want to face:\n");
            printf("--> otb: play over the board\n");
            printf("--> computer: play a computer\n");
            printf("2nd argument --> random (auto): choose 1st player randomly\n");
            printf("2nd argument --> 1: player 1 begins\n");
            printf("2nd argument --> 2: player 2 or computer begins\n");
            return 0;
        }
        else{
            printf("1st argument invalid\n");
            return 0;
        }
        
        if(argc == 3){
            if(!strcasecmp(argv[2], "random")) {
                firstPlayer = rand() % 2;    // Returns a pseudo-random integer between 0 and RAND_MAX -> modulo 2 will just return 1 or 2.
            }
            else if(!strcasecmp(argv[2], "1")) {
                firstPlayer = 0;
            }
            else if(!strcasecmp(argv[2], "2")) {
                firstPlayer = 1;
            }
            else {
                printf("2nd argument invalid");
                fflush(stdout);
                return 0;
            }
        }
        else {
            firstPlayer = rand() % 2;
        }
        
        printf("\nThe opponent is %s, good luck!\n", opponent);
        if(!strcmp(opponent, "alphaTicTac")){
            if(!firstPlayer){
                printf("You start!\n");
            }
            else {
                printf("The computer starts\n");
                roundsPlayed++;
                //computerPlaysRandomly();
                miniMaxInitCall(board, 1); // COMPUTER IS ALWAYS NR 2
                printBoard();
            }
        }
        else{
            printf("Player %d starts\n", firstPlayer + 1);
        }
        printf("***********************\n");
    }
    else if( argc > 3 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else {
        printf("One argument expected.\n");
        return 0;
    }
    
    while(boardFull(board) != 0 && !determinedWinner){
        roundsPlayed ++;
        printf("insert int #(1-9): ");
        fflush(stdout);
        int input;
        if (scanf("%d", &input) != 1) {
            perror("scanf failed");
            return 1;
        }
        if(input < 1 || input > 9){
            printf("please enter an integer from 1 to 9\n");
            roundsPlayed--;
            continue;
        }
        
        int *rowCol = numberToRowCol(input);

        if(board[rowCol[0]][rowCol[1]] != 0){
            printf("This square was already taken\n");
            roundsPlayed--;
            continue;
        }
        else{
            if (roundsPlayed % 2 == 1){
                board[rowCol[0]][rowCol[1]] = firstPlayer + 1;
            }
            else{
                if(!firstPlayer){
                    board[rowCol[0]][rowCol[1]] = 2;
                }
                else{
                    board[rowCol[0]][rowCol[1]] = 1;
                }
            }
        }

        printBoard();
        printf("***********************\n");
        fflush(stdout);

        checkGameConditions(rowCol[0], rowCol[1], board, 0);
        free(rowCol);

        if(!strcmp(opponent, "alphaTicTac") && !determinedWinner){
            roundsPlayed ++;
            //int *computerMove = computerPlaysRandomly();
            int *computerMove = miniMaxInitCall(board, 1);
            printBoard();
            printf("***********************\n");
            checkGameConditions(computerMove[0], computerMove[1], board, 0);
        }   
    }
    return 0;
}

void checkGameConditions(int row, int column, int boardInput[3][3], int depth) {
    int winner = checkWinner(row, column, boardInput, depth);
    if(winner == -1) {
        determinedWinner = 1;
        if(!strcmp(opponent, "alphaTicTac")){
            printf("You won!!!\n");
        }
        else {
            printf("Player 1 won!\n");
        }
        fflush(stdout);
    }
    else if(winner == 1){
        determinedWinner = 1;
        if(!strcmp(opponent, "alphaTicTac")){
            printf("You lost unfortunately :(\n");
        }
        else {
            printf("Player 2 won!\n");
        }
        fflush(stdout);
    }
    else {
        if(boardFull(boardInput) == 0){
            determinedWinner = 1;
            printf("Board is FULL, it's a draw\n");
            fflush(stdout);
        }
    }
}

int* computerPlaysRandomly() {
    // random time based on nano seconds
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);

    // IMPLEMENT MINIMAX ALGORITHM

    int *availableSpots = checkAvailableSpots(board);
    for(int i = 0; i < 9; i++){
        printf("%d ", availableSpots[i]);
    }
    int spotsLeft = boardFull(board);

    int chosenNumber = rand() % spotsLeft;
    int count = 0;
    int moveIndex = 0;
    for(int i = 0; i < 9; i++){
        if(availableSpots[i] == 0){
            count ++;
        }
        if(count == chosenNumber){
            moveIndex = i;
            break;
        }
    }

    int *rowCol = numberToRowCol(moveIndex + 1);

    printf("moveIndex: %d\n", moveIndex + 1);
    fflush(stdout);

    if (roundsPlayed % 2 == 1){
        board[rowCol[0]][rowCol[1]] = firstPlayer + 1;
    }
    else{
        if(!firstPlayer){
            board[rowCol[0]][rowCol[1]] = 2;
        }
        else{
            board[rowCol[0]][rowCol[1]] = 1;
        }
    }
    printf("The computer played on row %d and column %d\n", rowCol[0] + 1, rowCol[1] + 1);
    return rowCol;
}

void printBoard(){
    printf(" %i | %i | %i \n", board[0][0], board[0][1], board[0][2]);
    printf("---|---|--- \n");
    printf(" %i | %i | %i \n", board[1][0], board[1][1], board[1][2]);
    printf("---|---|--- \n");
    printf(" %i | %i | %i \n", board[2][0], board[2][1], board[2][2]);
}

void resetBoard(){
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            board[i][j] = 0;
        }
    }
}

int boardFull(int boardInput[3][3]) {
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

int checkWinner(int inputRow, int inputCol, int boardInput[3][3], int depth) {
    //If roundsPlayed int is smaller than 5 there can be no winner yet
    if(roundsPlayed + depth < 5) {
        return 0;
    }
    int amountInRow[2] = {0, 0};
    int amountInCol[2] = {0, 0};
    int amountInDiagonal1[2] = {0, 0};
    int amountInDiagonal2[2] = {0, 0};

    // Check row
    for(int i = 0; i < 3; i++){
        switch (boardInput[inputRow][i]){
            case 1:
                amountInRow[0]++;
                break;
            case 2:
                amountInRow[1]++;
                break;
            default:
                break;
        }
    }
    if(amountInRow[0] == 3)
        return -1;
    else if(amountInRow[1] == 3){
        return 1;
    }

    // Check column
    for(int i = 0; i < 3; i++){
        switch (boardInput[i][inputCol]){
            case 1:
                amountInCol[0]++;
                break;
            case 2:
                amountInCol[1]++;
                break;
            default:
                break;
        }
    }
    if(amountInCol[0] == 3)
        return -1;
    else if(amountInCol[1] == 3){
        return 1;
    }

    // Check diagonal 1
    for(int i = 0; i < 3; i++){
        switch (boardInput[i][i]){
            case 1:
                amountInDiagonal1[0]++;
                break;
            case 2:
                amountInDiagonal1[1]++;
                break;
            default:
                break;
        }
    }
    if(amountInDiagonal1[0] == 3)
        return -1;
    else if(amountInDiagonal1[1] == 3){
        return 1;
    }

    // Check diagonal 2
    for(int i = 0; i < 3; i++){
        switch (boardInput[2-i][i]){
            case 1:
                amountInDiagonal2[0]++;
                break;
            case 2:
                amountInDiagonal2[1]++;
                break;
            default:
                break;
        }
    }
    if(amountInDiagonal2[0] == 3)
        return -1;
    else if(amountInDiagonal2[1] == 3){
        return 1;
    }

    return 0;
}

int *checkAvailableSpots(int boardInput[3][3]) {
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

int *numberToRowCol(int number) {
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

// Function to be used to initialize the minimax algorithm
int *miniMaxInitCall(int boardInput[3][3], int player) {
    int *possibilities = checkAvailableSpots(boardInput);

    int *bestMove = (int *)malloc(2 * sizeof(int));
    if(player == 1){
        int best = -2; // lower than lowest possible score
        for(int i = 0; i < 9; i++){
            if(possibilities[i] == 0){
                int *move2 = numberToRowCol(i + 1);
                int score = miniMax(move2, boardInput, 1, 2);
                if(score > best){ // maximizing player
                    best = score;
                    memcpy(bestMove, move2, 2 * sizeof(int));
                }
            }
        }
    }
    else if(player == 2){
        int best = 2; // higher than highest possible score
        for(int i = 0; i < 9; i++){
            if(possibilities[i] == 0){
                int *move2 = numberToRowCol(i + 1);
                int score = miniMax(move2, boardInput, 1, 1);
                if(score < best){ // minimizing player
                    best = score;
                    memcpy(bestMove, move2, 2 * sizeof(int));
                }
            }
        }
    }

    if (roundsPlayed % 2 == 1){
        board[bestMove[0]][bestMove[1]] = firstPlayer + 1;
    }
    else{
        if(!firstPlayer){
            board[bestMove[0]][bestMove[1]] = 2;
        }
        else{
            board[bestMove[0]][bestMove[1]] = 1;
        }
    }
    printf("The computer played on row %d and column %d\n", bestMove[0] + 1, bestMove[1] + 1);

    free(possibilities);
    return bestMove;
}

int miniMax(int *move, int boardInput[3][3], int depth, int player){
    int boardCopy[3][3] = {{0}}; // Initialize all elements to zero
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            boardCopy[i][j] = boardInput[i][j];
        }
    }
    
    boardCopy[move[0]][move[1]] = player;
    int positionEval = checkWinner(move[0], move[1], boardCopy, depth);
    if (positionEval != 0){
        return positionEval;
    }
    
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
                if(score > best){ // maximizing player
                    best = score;
                }
            }
        }
        free(possibilities);
        return best;
    }
    else if(player == 2){
        int best = 2; // higher than highest possible score
        for(int i = 0; i < 9; i++){
            if(possibilities[i] == 0){
                int *move2 = numberToRowCol(i + 1);
                int score = miniMax(move2, boardCopy, depth + 1, 1);
                if(score < best){ // minimizing player
                    best = score;
                }
            }
        }
        free(possibilities);
        return best;
    }
}
