#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "Minesweeper.h"

typedef struct {
    char name[50];
    int score;
} Player;

Player topScores[TOP_PLAYERS];

void initializeTopScores() {
    for (int i = 0; i < TOP_PLAYERS; i++) {
        strcpy(topScores[i].name, "---");
        topScores[i].score = 0;
    }
}

void displayTopScores() {
    printf("\n=== Top Scores ===\n");
    for (int i = 0; i < TOP_PLAYERS; i++) {
        printf("%d. %s - %d\n", i + 1, topScores[i].name, topScores[i].score);
    }
    printf("\n");
}

void updateTopScores(const char *name, int score) {
    for (int i = 0; i < TOP_PLAYERS; i++) {
        if (score > topScores[i].score) {
            for (int j = TOP_PLAYERS - 1; j > i; j--) {
                topScores[j] = topScores[j - 1];
            }
            strcpy(topScores[i].name, name);
            topScores[i].score = score;
            break;
        }
    }
}

void saveTopScoresToFile() {
    FILE *file = fopen("wyniki.txt", "w");
    if (file == NULL) {
        perror("Failed to open wyniki.txt for saving.");
        return;
    }

    for (int i = 0; i < TOP_PLAYERS; i++) {
        fprintf(file, "%s %d\n", topScores[i].name, topScores[i].score);
    }
    fclose(file);
}

void loadTopScoresFromFile() {
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL) {
        perror("Failed to open scores.txt for loading.");
        return;
    }

    for (int i = 0; i < TOP_PLAYERS; i++) {
        if (fscanf(file, "%49s %d", topScores[i].name, &topScores[i].score) != 2) {
            strcpy(topScores[i].name, "---");
            topScores[i].score = 0;
        }
    }
    fclose(file);
}

void initializeBoard(char board[][30], int rows, int cols, int mines) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '0';
        }
    }

    for (int i = 0; i < mines; i++) {
        int r, c;
        do {
            r = rand() % rows;
            c = rand() % cols;
        } while (board[r][c] == '*');
        board[r][c] = '*';
    }
}

void displayBoard(char board[][30], char mask[][30], int rows, int cols) {
    printf("   ");
    for (int i = 0; i < cols; i++) {
        printf("%2d", i);
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("%2d ", i);
        for (int j = 0; j < cols; j++) {
            if (mask[i][j] == 'F') {
                printf(" F");
            } else if (mask[i][j] == '1') {
                printf("%2c", board[i][j]);
            } else {
                printf(" #");
            }
        }
        printf("\n");
    }
}

bool isValid(int row, int col, int rows, int cols) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

int countAdjacentMines(char board[][30], int row, int col, int rows, int cols) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (isValid(row + i, col + j, rows, cols) && board[row + i][col + j] == '*') {
                count++;
            }
        }
    }
    return count;
}

bool revealCell(char board[][30], char mask[][30], int row, int col, int rows, int cols, int *revealedCells) {
    if (!isValid(row, col, rows, cols) || mask[row][col] == '1' || mask[row][col] == 'F') {
        return false;
    }

    mask[row][col] = '1';
    (*revealedCells)++;

    if (board[row][col] == '*') {
        return true;
    }

    int mines = countAdjacentMines(board, row, col, rows, cols);
    board[row][col] = mines + '0';

    if (mines == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    revealCell(board, mask, row + i, col + j, rows, cols, revealedCells);
                }
            }
        }
    }

    return false;
}

void revealAllMines(char board[][30], char mask[][30], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == '*') {
                mask[i][j] = '1';
            }
        }
    }
}

void toggleFlag(char mask[][30], int row, int col) {
    if (mask[row][col] == '1') {
        printf("Cannot place a flag on a revealed cell.\n");
    } else if (mask[row][col] == 'F') {
        mask[row][col] = '0';
        printf("Flag removed from (%d, %d).\n", row, col);
    } else if (mask[row][col] == '0' || mask[row][col] == 0) {
        mask[row][col] = 'F';
        printf("Flag placed on (%d, %d).\n", row, col);
    } else {
        printf("Unexpected cell state at (%d, %d).\n", row, col);
    }
}

void loadCustomBoard(char board[][30], int *rows, int *cols, int *mines) {
    printf("Enter the number of rows (max 16): ");
    scanf("%d", rows);
    printf("Enter the number of columns (max 30): ");
    scanf("%d", cols);

    int maxMines = (*rows) * (*cols) - 1;
    printf("Enter the number of mines (max %d): ", maxMines);
    scanf("%d", mines);

    if (*rows > 16) *rows = 16;
    if (*cols > 30) *cols = 30;
    if (*mines > maxMines) *mines = maxMines;
    if (*mines <= 0) *mines = 1;

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            board[i][j] = '0';
        }
    }

    for (int i = 0; i < *mines; i++) {
        int r, c;
        do {
            r = rand() % (*rows);
            c = rand() % (*cols);
        } while (board[r][c] == '*');
        board[r][c] = '*';
    }
}

int playGame(int difficulty) {
    int rows, cols, mines, multiplier;
    char board[16][30];
    char mask[16][30] = {0};

    if (difficulty == EASY) {
        rows = EASY_ROWS;
        cols = EASY_COLS;
        mines = EASY_MINES;
        multiplier = 1;
        initializeBoard(board, rows, cols, mines);
    } else if (difficulty == MEDIUM) {
        rows = MEDIUM_ROWS;
        cols = MEDIUM_COLS;
        mines = MEDIUM_MINES;
        multiplier = 2;
        initializeBoard(board, rows, cols, mines);
    } else if (difficulty == HARD) {
        rows = HARD_ROWS;
        cols = HARD_COLS;
        mines = HARD_MINES;
        multiplier = 3;
        initializeBoard(board, rows, cols, mines);
    } else if (difficulty == CUSTOM) {
        loadCustomBoard(board, &rows, &cols, &mines);
        multiplier = 1;
    }

    int revealedCells = 0;
    int totalCells = rows * cols - mines;
    bool gameOver = false;

    while (!gameOver) {
        displayBoard(board, mask, rows, cols);
        printf("Revealed cells: %d/%d\n", revealedCells, totalCells);

        char action;
        int r, c;
        printf("Enter action (f for flag, r for reveal) and coordinates (e.g., f 0 0 or r 0 0): ");
        scanf(" %c %d %d", &action, &r, &c);

        if (!isValid(r, c, rows, cols)) {
            printf("Invalid coordinates. Try again.\n");
            continue;
        }

        if (action == 'f') {
            toggleFlag(mask, r, c);
        } else if (action == 'r') {
            if (mask[r][c] == 'F') {
                printf("Cannot reveal a flagged cell. Remove the flag first.\n");
                continue;
            }

            if (revealCell(board, mask, r, c, rows, cols, &revealedCells)) {
                printf("You hit a mine! Game over.\n");
                revealAllMines(board, mask, rows, cols);
                displayBoard(board, mask, rows, cols);
                gameOver = true;
            } else {
                if (revealedCells == totalCells) {
                    printf("Congratulations! You cleared the board.\n");
                    gameOver = true;
                }
            }
        } else {
            printf("Invalid action. Use 'f' for flag or 'r' for reveal.\n");
        }
    }

    int score = revealedCells * multiplier;
    printf("Your score: %d\n", score);

    char name[50];
    printf("Enter your name: ");
    scanf("%s", name);

    updateTopScores(name, score);

    return score;
}

void loadMovesFromFile() {
    FILE *file = fopen("moves.txt", "r");
    if (file == NULL) {
        perror("Failed to open moves.txt for loading.");
        return;
    }

    int difficulty;
    if (fscanf(file, "%d", &difficulty) != 1) {
        printf("Invalid format in moves.txt.\n");
        fclose(file);
        return;
    }

    int rows, cols, mines;
    char board[16][30];
    char mask[16][30] = {0};

    if (difficulty == EASY) {
        rows = EASY_ROWS;
        cols = EASY_COLS;
        mines = EASY_MINES;
        initializeBoard(board, rows, cols, mines);
    } else if (difficulty == MEDIUM) {
        rows = MEDIUM_ROWS;
        cols = MEDIUM_COLS;
        mines = MEDIUM_MINES;
        initializeBoard(board, rows, cols, mines);
    } else if (difficulty == HARD) {
        rows = HARD_ROWS;
        cols = HARD_COLS;
        mines = HARD_MINES;
        initializeBoard(board, rows, cols, mines);
    } else {
        printf("Invalid difficulty level in moves.txt.\n");
        fclose(file);
        return;
    }

    int revealedCells = 0;
    int totalCells = rows * cols - mines;
    bool gameOver = false;

    char action;
    int r, c;

    while (fscanf(file, " %c %d %d", &action, &r, &c) == 3) {
        if (!isValid(r, c, rows, cols)) {
            printf("Invalid move (%c %d %d) in moves.txt.\n", action, r, c);
            continue;
        }

        if (gameOver) break;

        if (action == 'f') {
            toggleFlag(mask, r, c);
        } else if (action == 'r') {
            if (mask[r][c] == 'F') {
                printf("Cannot reveal a flagged cell. Remove the flag first.\n");
                continue;
            }

            if (revealCell(board, mask, r, c, rows, cols, &revealedCells)) {
                printf("You hit a mine! Game over.\n");
                revealAllMines(board, mask, rows, cols);
                displayBoard(board, mask, rows, cols);
                gameOver = true;
            } else if (revealedCells == totalCells) {
                printf("Congratulations! You cleared the board.\n");
                gameOver = true;
            }
        }
    }

    fclose(file);
    printf("=====================\n");
    if (gameOver && revealedCells == totalCells) {
        printf("You won!\n");
    } else if (gameOver) {
        printf("You lost...\n");
    } else {
        printf("Game hasn't been finished \n");
    }
    printf("=====================\n\n");

}
