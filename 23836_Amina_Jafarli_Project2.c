
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 8
#define COLS 8

#define TREASURES 5
#define TRAPS 3

#define STARTING_LIVES 3

#define EMPTY 0
#define TREASURE 1
#define TRAP -1

#define HIDDEN 0
#define REVEALED 1

#define HIDDEN_SYMBOL '#'

int isInside(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

void initializeBoards(int board[ROWS][COLS], int revealed[ROWS][COLS]) {
    int row, col;

    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            board[row][col] = EMPTY;
            revealed[row][col] = HIDDEN;
        }
    }
}

void placeObjects(int board[ROWS][COLS]) {
    int i = 0;
    int row, col;

    while (i < TREASURES) {
        row = rand() % ROWS;
        col = rand() % COLS;

        if (board[row][col] == EMPTY) {
            board[row][col] = TREASURE;
            i++;
        }
    }
    i = 0;

    while (i < TRAPS) {
        row = rand() % ROWS;
        col = rand() % COLS;

        if (board[row][col] == EMPTY) {
            board[row][col] = TRAP;
            i++;
        }
    }
}

int countNearbyTreasures(int board[ROWS][COLS], int row, int col) {
    int r, c;
    int count = 0;

    for (r = row - 1; r <= row + 1; r++) {
        for (c = col - 1; c <= col + 1; c++) {
            if (isInside(r, c) &&
                !(r == row && c == col)) {
                if (board[r][c] == TREASURE) {
                    count++;
                }
            }
        }
    }

    return count;
}

int countTreasuresInRegion(int board[ROWS][COLS], int row, int col) {
    int r, c;
    int count = 0;

    for (r = row - 1; r <= row + 1; r++) {
        for (c = col - 1; c <= col + 1; c++) {
            if (isInside(r, c) &&
                board[r][c] == TREASURE) {
                count++;
            }
        }
    }
    return count;
}

int countTrapsInRegion(int board[ROWS][COLS], int row, int col) {
    int r, c;
    int count = 0;

    for (r = row - 1; r <= row + 1; r++) {
        for (c = col - 1; c <= col + 1; c++) {
            if (isInside(r, c) && board[r][c] == TRAP) {
                count++;
            }
        }
    }
    return count;
}

void printBoard(int board[ROWS][COLS], int revealed[ROWS][COLS], int showAll) {
    printf("    ");

    for (int c = 0; c < COLS; c++) {
        printf("%d ", c);
    }

    printf("\n");

    for (int r = 0; r < ROWS; r++) {
        printf("%d | ", r);

        for (int c = 0; c < COLS; c++) {
            if (showAll) {
                if (board[r][c] == TREASURE) {
                    printf("T ");
                } else if (board[r][c] == TRAP) {
                    printf("X ");
                } else {
                    printf("0 ");
                }
            }
            else if (revealed[r][c] == HIDDEN) {
                printf("%c ", HIDDEN_SYMBOL);
            } else if (board[r][c] == TREASURE) {
                printf("T ");
            } else if (board[r][c] == TRAP) {
                printf("X ");
            } else {
                printf("%d ", countNearbyTreasures(board, r, c));
            }
        }
        printf("\n");
    }
}

int revealCell(int board[ROWS][COLS], int revealed[ROWS][COLS], int row, int col) {
    revealed[row][col] = REVEALED;
    return board[row][col];
}

void printStatus(int lives, int treasures_num, int scanner) {
    printf("\nLives: %d   Treasures: %d/%d   Scanner: ", lives, treasures_num, TREASURES);

    if (scanner == 0) {
        printf("available\n");
    } else {
        printf("used\n");
    }
}

void useScanner(int board[ROWS][COLS]) {
    int row, col;

    while (1) {

        printf("Scanner center row: ");
        scanf("%d", &row);

        printf("Scanner center column: ");
        scanf("%d", &col);

        if (!isInside(row, col)) {
            printf("Invalid coordinates. Enter values from 0 to 7.\n");
            continue;
        }
        break;
    }

    printf("\nScanner result: %d treasures and %d traps in this region.\n", countTreasuresInRegion(board, row, col), countTrapsInRegion(board, row, col));
}

int main() {

    int board[ROWS][COLS];
    int revealed[ROWS][COLS];

    int lives = STARTING_LIVES;
    int treasures_num = 0;
    int scanner = 0;

    int row, col;
    int result;
    char action;

    srand(time(NULL));

    initializeBoards(board, revealed);
    placeObjects(board);

    printf("========== TREASURE HUNT ==========\n");

    while (lives > 0 && treasures_num < TREASURES) {
        printBoard(board, revealed, 0);
        printStatus(lives, treasures_num, scanner);

        printf("\nAction (R = reveal, S = scanner): ");
        scanf(" %c", &action);

        if (action == 'R' || action == 'r') {

            printf("Row: ");
            scanf("%d", &row);

            printf("Column: ");
            scanf("%d", &col);

            if (!isInside(row, col)) {
                printf("Invalid coordinates. Enter values from 0 to 7.\n");
                continue;
            }

            if (revealed[row][col] == REVEALED) {
                printf("This cell has already been explored.\n");
                continue;
            }
            result = revealCell(board, revealed, row, col);

            if (result == TREASURE) {
                treasures_num++;
                printf("You found a treasure!\n");
            } else if (result == TRAP) {
                lives--;
                printf("Trap! You lost one life.\n");
            } else {
                printf("Empty cell. There are %d nearby treasures.\n",
                       countNearbyTreasures(board, row, col));
            }
        } else if (action == 'S' || action == 's') {

            if (scanner == 1) {
                printf("Scanner is unavailable. It has already been used.\n");
                continue;
            }

            useScanner(board);
            scanner = 1;
        } else {
            printf("Invalid action. Please choose R or S.\n");
        }
    }

    printf("\n========== GAME OVER ==========\n");

    if (treasures_num == TREASURES) {
        printf("Congratulations! You found all five treasures. YOU WIN!\n");
    } else {
        printf("You lost all your lives. GAME OVER!\n");
    }

    printf("\nComplete board:\n");
    printBoard(board, revealed, 1);

    return 0;
}
