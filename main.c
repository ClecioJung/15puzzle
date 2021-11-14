/*
 *------------------------------------------------------------------------------
 * LIBRARIES
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/*
 *------------------------------------------------------------------------------
 * DEFINITIONS
 *------------------------------------------------------------------------------
 */

#define SIDES 4
#define SQUARED_SIDES (SIDES * SIDES)
#define PIECES (SIDES * SIDES - 1)

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

void printBoard(void)
{
    for (unsigned int i = 0; i < SQUARED_SIDES; i++)
    {
        if ((i % SIDES) == 0)
        {
            if (i != 0)
            {
                putchar('|');
            }
            printf("\n ");
            for (unsigned int j = 0; j < SIDES; j++)
            {
                printf("----");
#if (SIDES > 3)
                putchar('-');
#endif
            }
            printf("-\n |");
        }
        else
        {
            printf("|");
        }
        if (board[i] == 0)
        {
            printf("   ");
#if (SIDES > 3)
            putchar(' ');
#endif
        }
        else
        {
#if (SIDES < 4)
            printf(" %d ", board[i]);
#else
            printf(" %02d ", board[i]);
#endif
        }
    }
    printf("|\n ");
    for (unsigned int j = 0; j < SIDES; j++)
    {
        printf("----");
#if (SIDES > 3)
        putchar('-');
#endif
    }
    printf("-\n ");
}

void rewindCursor(const unsigned int lines, const unsigned int columns)
{
    printf("\x1b[%dA", lines);
    printf("\x1b[%dD", columns);
}

#define ARROW_UP 'A'
#define ARROW_DOWM 'B'
#define ARROW_RIGHT 'C'
#define ARROW_LEFT 'D'

char getArrowKey(void)
{
    while (getchar() != '\033')
    {
    }
    getchar();
    return getchar();
}

void initBoardGame(void)
{
    bool usedNumbers[SQUARED_SIDES];
    memset(usedNumbers, false, sizeof(usedNumbers));
    srand((unsigned int)time(NULL));
    for (unsigned int i = 0; i < SQUARED_SIDES; i++)
    {
        unsigned int randomNumber = rand() % SQUARED_SIDES;
        if (!usedNumbers[randomNumber])
        {
            board[i] = randomNumber;
            usedNumbers[randomNumber] = true;
        }
        else
        {
            i--;
        }
    }
}

unsigned int findEmptyPosition(void)
{
    for (unsigned int emptyPosition = 0; emptyPosition < SQUARED_SIDES; emptyPosition++)
    {
        if (!board[emptyPosition])
        {
            return emptyPosition;
        }
    }
    return 0;
}

void getPlayerMove(void)
{
    const unsigned int emptyPosition = findEmptyPosition();
    while (true)
    {
        switch (getArrowKey())
        {
        case ARROW_UP:
            if (emptyPosition < (SQUARED_SIDES - SIDES))
            {
                board[emptyPosition] = board[(emptyPosition + SIDES)];
                board[(emptyPosition + SIDES)] = 0;
                return;
            }
            break;
        case ARROW_DOWM:
            if (emptyPosition >= SIDES)
            {
                board[emptyPosition] = board[(emptyPosition - SIDES)];
                board[(emptyPosition - SIDES)] = 0;
                return;
            }
            break;
        case ARROW_RIGHT:
            if ((emptyPosition % SIDES) != 0)
            {
                board[emptyPosition] = board[(emptyPosition - 1)];
                board[(emptyPosition - 1)] = 0;
                return;
            }
            break;
        case ARROW_LEFT:
            if ((emptyPosition % SIDES) != (SIDES - 1))
            {
                board[emptyPosition] = board[(emptyPosition + 1)];
                board[(emptyPosition + 1)] = 0;
                return;
            }
            break;
        }
    }
}

bool continueGame(void)
{
    for (unsigned int i = 0; i < (SQUARED_SIDES - 1); i++)
    {
        if (board[i] != (i + 1))
        {
            return true;
        }
    }
    return false;
}

void restoreTerminal(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void configureTerminal(void)
{
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ICANON;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    atexit(restoreTerminal);
}

/*
 *------------------------------------------------------------------------------
 * MAIN
 *------------------------------------------------------------------------------
 */

int main(void)
{
    printf("Instructions: Use keyboard arrow keys to move the pieces!\n");
    configureTerminal();
    initBoardGame();
    printBoard();
    while (continueGame())
    {
        getPlayerMove();
        rewindCursor(2 * (SIDES + 1), 0);
        printBoard();
    }
    printf("\nCongratulations! You ended the game!\n");
    return EXIT_SUCCESS;
}

/*
 *------------------------------------------------------------------------------
 * END
 *------------------------------------------------------------------------------
 */
