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

    if (roundsPlayed % 2 == 1)
        board[rowCol[0]][rowCol[1]] = firstPlayer + 1;
    else
	{
        if(!firstPlayer)
            board[rowCol[0]][rowCol[1]] = 2;
        else
            board[rowCol[0]][rowCol[1]] = 1;
    }
    printf("The computer played on row %d and column %d\n", rowCol[0] + 1, rowCol[1] + 1);
    return rowCol;
}