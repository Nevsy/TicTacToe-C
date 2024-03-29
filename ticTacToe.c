#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

void printBoard();
void resetBoard();
int boardFull();
int checkWinner(int inputRow, int inputCol);
int * computerPlays();
void checkGameConditions(int row, int column);

int board[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}};
//bool determinedWinner = false; // Game loop runs until true
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
        if(!strcmp(argv[1], "otb")){
            strcpy(opponent, "Player 2");
        }
        else if(!strcmp(argv[1], "computer")){
            strcpy(opponent, "alphaTicTac");
        }
        else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")){
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
            if(!strcmp(argv[2], "random")) {
                firstPlayer = rand() % 2;    // Returns a pseudo-random integer between 0 and RAND_MAX -> modulo 2 will just return 1 or 2.
            }
            else if(!strcmp(argv[2], "1")) {
                firstPlayer = 0;
            }
            else if(!strcmp(argv[2], "2")) {
                firstPlayer = 1;
            }
            else {
                printf("2nd argument invalid");
                fflush(stdout);
                return 0;
            }
            //printf("%d: strcomp", strcmp(argv[2], "1"));
        }
        else {
            firstPlayer = rand() % 2;
        }
        
        printf("\nThe opponent is %s, good luck!\n", opponent);
        if(!strcmp(opponent, "alphaTicTac")){
            if(firstPlayer){
              printf("You start!\n");
            }
            else {
                printf("The computer starts\n");
                roundsPlayed++;
                computerPlays(roundsPlayed);
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
    
    while(boardFull() == 0 && !determinedWinner){
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
        int column; //cleverly determine the columns place
        if(input % 3 == 0) {
            column = 2;
        }
        else{
            column = input % 3 - 1;
        }

        int row; // brute force the row place
        if(input < 4){
          row = 0;
        }
        else if (input < 7){
            row = 1;
        }
        else {
            row = 2;
        }

        if(board[row][column] != 0){
            printf("This square was already taken\n");
            roundsPlayed--;
            continue;
        }
        else{
            if (roundsPlayed % 2 == 1){
                //printf("This, %d\n", firstPlayer);
                board[row][column] = firstPlayer + 1;
            }
            else{
                if(!firstPlayer){
                    board[row][column] = 2;
                }
                else{
                    board[row][column] = 1;
                }
            }
        }

        printBoard();
        printf("***********************\n");
        fflush(stdout);

        checkGameConditions(row, column);

        if(!strcmp(opponent, "alphaTicTac") && !determinedWinner){
            roundsPlayed ++;
            int *computerMove = computerPlays();
            printBoard();
            printf("***********************\n");
            checkGameConditions(computerMove[0], computerMove[1]);
        }
    }
    //free(opponent);
    return 0;
}

void checkGameConditions(int row, int column){
    int winner = checkWinner(row, column);
    if(winner == 1) {
        determinedWinner = 1;
        if(!strcmp(opponent, "alphaTicTac")){
            printf("You lost unfortunately :(\n");
        }
        else {
            printf("Player 1 won!\n");
        }
        fflush(stdout);
    }
    else if(winner == 2){
        determinedWinner = 1;
        if(!strcmp(opponent, "alphaTicTac")){
            printf("You won!!!\n");
        }
        else {
            printf("Player 2 won!\n");
        }
        fflush(stdout);
    }
    else {
        if(boardFull() == 1){
            printf("Board is FULL, it's a draw\n");
            fflush(stdout);
        }
    }
}

int* computerPlays() {
    // IMPLEMENT MINIMAX ALGORITHM

    static int move[2];
    int moveRow = rand() % 3;
    int moveColumn = rand() % 3;
    while(board[moveRow][moveColumn] != 0){
        moveRow = rand() % 3;
        moveColumn = rand() % 3;
    }
    move[0] = moveRow;
    move[1] = moveColumn;
    if (roundsPlayed % 2 == 1){
        board[moveRow][moveColumn] = firstPlayer + 1;
    }
    else{
        if(!firstPlayer){
            board[moveRow][moveColumn] = 2;
        }
        else{
            board[moveRow][moveColumn] = 1;
        }
    }
    //board[moveRow][moveColumn] = firstPlayer + 1;
    printf("The computer played on row %d and column %d\n", moveRow + 1, moveColumn + 1);
    return move;
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

int boardFull() {
    int empty = 9;
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if(board[i][j] != 0)
            empty--;
        }
    }
    
    if(empty == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int checkWinner(int inputRow, int inputCol) {
    //If roundsPlayed int is smaller than 6 (6 not included) there can be no winner yet
    if(roundsPlayed < 6) {
        return 0;
    }
    int amountInRow[2] = {0, 0};
    int amountInCol[2] = {0, 0};
    int amountInDiagonal1[2] = {0, 0};
    int amountInDiagonal2[2] = {0, 0};

    for(int i = 0; i < 3; i++){
        switch (board[inputRow][i]){
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
        return 1;
    else if(amountInRow[1] == 3){
        return 2;
    }
    else {
        for(int i = 0; i < 3; i++){
            switch (board[i][inputCol]){
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
            return 1;
        else if(amountInCol[1] == 3){
            return 2;
        }
        else {
            for(int i = 0; i < 3; i++){
                switch (board[i][i]){
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
                return 1;
            else if(amountInDiagonal1[1] == 3){
                return 2;
            }
            else {
                for(int i = 0; i < 3; i++){
                    switch (board[2-i][i]){
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
                    return 1;
                else if(amountInDiagonal2[1] == 3){
                    return 2;
                }
                else {
                    return 0;
                }
            }
        }
    }
}
