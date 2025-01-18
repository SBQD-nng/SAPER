#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "Minesweeper.h"
int main() {
    int choice;

    srand(time(NULL));
    initializeTopScores();
    loadTopScoresFromFile();

    while (1) {
        printf("=== Minesweeper ===\n");
        printf("1. Easy (9x9, 10 mines)\n");
        printf("2. Medium (16x16, 40 mines)\n");
        printf("3. Hard (16x30, 99 mines)\n");
        printf("4. Custom Board\n");
        printf("5. View Top Scores\n");
        printf("6. Load Moves from File\n");
        printf("7. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            case 2:
            case 3:
                playGame(choice);
                break;
            case 4:
                playGame(CUSTOM);
                break;
            case 5:
                displayTopScores();
                break;
            case 6:
                loadMovesFromFile();
                break;
            case 7:
                saveTopScoresToFile();
                printf("Exiting the game. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
