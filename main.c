/*
 *------------------------------------------------------------------------------
 * LIBRARIES
 *------------------------------------------------------------------------------
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/*
 *------------------------------------------------------------------------------
 * DEFINITIONS
 *------------------------------------------------------------------------------
 */

#define SIDES 4
#define SQUARED_SIDES (SIDES * SIDES)
#define PIECES (SIDES * SIDES - 1)
#if PIECES < 10
#define DIGITS 1
#elif PIECES < 100
#define DIGITS 2
#else
#error "Usnupported number of pieces!"
#endif

/*
 *------------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *------------------------------------------------------------------------------
 */

struct termios oldt, newt;

unsigned int board[SQUARED_SIDES];

/*
 *------------------------------------------------------------------------------
 * FUNCTIONS
 *------------------------------------------------------------------------------
 */

void printBoard(void) {
    for (unsigned int i = 0; i < SQUARED_SIDES; i++) {
        if ((i % SIDES) == 0) {
            if (i != 0) {
                putchar('|');
            }
            printf("\n ");
            for (unsigned int j = 0; j < SIDES; j++) {
                printf("%.*s", DIGITS+3, "----------");
            }
            printf("-\n |");
        } else {
            printf("|");
        }
        if (board[i] == 0) {
            printf("%*s", DIGITS+2, "");
        } else {
            printf(" %.*d ", DIGITS, board[i]);
        }
    }
    printf("|\n ");
    for (unsigned int j = 0; j < SIDES; j++) {
        printf("%.*s", DIGITS+3, "----------");
    }
    printf("-\n ");
}

void rewindCursor(const unsigned int lines, const unsigned int columns) {
    printf("\x1b[%dA", lines);
    printf("\x1b[%dD", columns);
}

#define ARROW_UP 'A'
#define ARROW_DOWM 'B'
#define ARROW_RIGHT 'C'
#define ARROW_LEFT 'D'

char getArrowKey(void) {
    while (getchar() != '\033') {
    }
    getchar();
    return (char)getchar();
}

unsigned int findEmptyPosition(void) {
    for (unsigned int emptyPosition = 0; emptyPosition < SQUARED_SIDES; emptyPosition++) {
        if (!board[emptyPosition]) {
            return emptyPosition;
        }
    }
    return 0;
}

// Count the number of permutations necessary to put the game board in the right order
unsigned int permutations(void) {
    unsigned int n = 0;
    for (unsigned int i = 0; (i+1) < SQUARED_SIDES; i++) {
        if (board[i] != 0) {
            for (unsigned int j = (i+1); j < SQUARED_SIDES; j++) {
                if ((board[j] != 0) && (board[i] > board[j])) {
                    n++;
                }
            }
        }
    }
    return n;
}

// https://mathworld.wolfram.com/15Puzzle.html
// https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/
bool isSolvable(void) {
#if ((SIDES % 2) != 0) // odd
    return ((permutations() % 2) == 0);
#else // even
    const unsigned int emptyPosition = findEmptyPosition();
    const unsigned int row = emptyPosition / SIDES + 1;
    return ((row % 2) == 0) != ((permutations() % 2) != 0);
#endif
}

void initBoardGame(void) {
    bool usedNumbers[SQUARED_SIDES];
    srand((unsigned int)time(NULL));
    do {
        memset(usedNumbers, false, sizeof(usedNumbers));
        for (unsigned int i = 0; i < SQUARED_SIDES; i++) {
            unsigned int randomNumber = (unsigned int)rand() % SQUARED_SIDES;
            if (!usedNumbers[randomNumber]) {
                board[i] = randomNumber;
                usedNumbers[randomNumber] = true;
            } else {
                i--;
            }
        }
    } while (!isSolvable());
}

void getPlayerMove(void) {
    const unsigned int emptyPosition = findEmptyPosition();
    while (true) {
        switch (getArrowKey()) {
            case ARROW_UP:
                if (emptyPosition < (SQUARED_SIDES - SIDES)) {
                    board[emptyPosition] = board[(emptyPosition + SIDES)];
                    board[(emptyPosition + SIDES)] = 0;
                    return;
                }
                break;
            case ARROW_DOWM:
                if (emptyPosition >= SIDES) {
                    board[emptyPosition] = board[(emptyPosition - SIDES)];
                    board[(emptyPosition - SIDES)] = 0;
                    return;
                }
                break;
            case ARROW_RIGHT:
                if ((emptyPosition % SIDES) != 0) {
                    board[emptyPosition] = board[(emptyPosition - 1)];
                    board[(emptyPosition - 1)] = 0;
                    return;
                }
                break;
            case ARROW_LEFT:
                if ((emptyPosition % SIDES) != (SIDES - 1)) {
                    board[emptyPosition] = board[(emptyPosition + 1)];
                    board[(emptyPosition + 1)] = 0;
                    return;
                }
                break;
        }
    }
}

bool continueGame(void) {
    for (unsigned int i = 0; i < (SQUARED_SIDES - 1); i++) {
        if (board[i] != (i + 1)) {
            return true;
        }
    }
    return false;
}

void restoreTerminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void configureTerminal(void) {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= (unsigned int)~ICANON;
    newt.c_lflag &= (unsigned int)~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    atexit(restoreTerminal);
}

/*
 *------------------------------------------------------------------------------
 * MAIN
 *------------------------------------------------------------------------------
 */

int main(void) {
    printf("Instructions: Use keyboard arrow keys to move the pieces!\n");
    configureTerminal();
    initBoardGame();
    printf("Number of permutations: %u\n", permutations());
    printBoard();
    while (continueGame()) {
        getPlayerMove();
        rewindCursor(2 * (SIDES + 1), 0);
        printBoard();
    }
    printf("\nCongratulations! You solved the puzzle!\n");
    return EXIT_SUCCESS;
}

/*
 *------------------------------------------------------------------------------
 * END
 *------------------------------------------------------------------------------
 */
