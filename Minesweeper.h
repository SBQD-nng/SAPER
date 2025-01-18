#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <stdbool.h>

#define EASY 1
#define MEDIUM 2
#define HARD 3
#define CUSTOM 4

#define EASY_ROWS 9
#define EASY_COLS 9
#define EASY_MINES 10

#define MEDIUM_ROWS 16
#define MEDIUM_COLS 16
#define MEDIUM_MINES 40

#define HARD_ROWS 16
#define HARD_COLS 30
#define HARD_MINES 99

#define TOP_PLAYERS 5

void initializeTopScores();
void displayTopScores();
void updateTopScores(const char *name, int score);
void saveTopScoresToFile();
void loadTopScoresFromFile();
void initializeBoard(char board[][30], int rows, int cols, int mines);
void displayBoard(char board[][30], char mask[][30], int rows, int cols);
bool isValid(int row, int col, int rows, int cols);
int countAdjacentMines(char board[][30], int row, int col, int rows, int cols);
bool revealCell(char board[][30], char mask[][30], int row, int col, int rows, int cols, int *revealedCells);
void revealAllMines(char board[][30], char mask[][30], int rows, int cols);
void toggleFlag(char mask[][30], int row, int col);
int playGame(int difficulty);
void loadCustomBoard(char board[][30], int *rows, int *cols, int *mines);
void loadMovesFromFile();
#endif
