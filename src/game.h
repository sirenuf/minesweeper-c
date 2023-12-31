#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define X_AMOUNT 9
#define Y_AMOUNT 9
#define MINES    10

/* If the compiler is running on Windows. */
#ifdef _WIN32
    #include "win32/cls_windows.h"
#else
    #define clrscr() printf("\e[1;1H\e[2J")
#endif

enum Commands {
    QUIT = -1,
    FLAG,
    REVEAL,
    UP,
    DOWN,
    RIGHT,
    LEFT
};

const char* is_selected(bool Selected)
{
    if (Selected)
        return "\033[0;47m";
    
    return "";
}

enum Status {
    EXIT,
    RUNNING,
    LOST,
    WON
};

struct Cell
{
    bool IsMine;
    bool IsRevealed;
    bool IsSelected;
    bool IsFlagged;
    int  NeighbourMines;
};

struct Cell board[X_AMOUNT][Y_AMOUNT];

/* Store colour codes */
const int ColourCodes[9][2] = { 
    { 34, false }, // Blue
    { 32, false }, // Green
    { 31, false }, // Red
    { 34, true  }, // Dark blue
    { 31, true  }, // Dark red
    { 36, false }, // Cyan
    { 37, true  }, // Black (dark white)
    { 37, false }, // Grey  (white)
    { 33, false }  // Yellow (reserved for flags)
};

void init_board(void)
{
    /* Reset board structs. */
    for (int x = 0; x < X_AMOUNT; x++) {
        for (int y = 0; y < Y_AMOUNT; y++) {
            memset(&board[x][y], 0, sizeof(board[x][y]));
        }
    }

    /* Random number seed. */
    srand(time(0));

    /* Install mines across random cells. */
    int MinesToSet = MINES;
    while (MinesToSet > 0) {
        /* Generate random cell location */
        int x = rand() % X_AMOUNT;
        int y = rand() % Y_AMOUNT;

        struct Cell *target = &board[x][y];
        
        if (target->IsMine)
            continue;
        
        target->IsMine = true;
        MinesToSet--;
    }

    /* Store the amount of mines that's near a cell. */
    for (int y = 0; y < Y_AMOUNT; y++) {
        for (int x = 0; x < X_AMOUNT; x++) {
            struct Cell *Target = &board[x][y];

            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                for (int offsetX = -1; offsetX <= 1; offsetX++) {
                    /* 
                     *  Make sure the cell check doesn't go out of bounds
                     *  E.g; Y: -1 / Y: Y_AMOUNT + 1
                     */
                    if ( y+offsetY >= 0 && y+offsetY < Y_AMOUNT &&
                         x+offsetX >= 0 && x+offsetX < X_AMOUNT ) {
                        /*
                         *  If the offset cell is a mine then 
                         *  increment the target cell's NeigbourMines.
                         */
                        if (board[x+offsetX][y+offsetY].IsMine)
                            Target->NeighbourMines++;
                    }
                }
            }
        }
    }
}

void draw_board(void)
{
    clrscr();

    /* Draws the top border. */
    printf(" ┌");
    for (int x = 0; x < X_AMOUNT; x++) {
        if (x+1 == X_AMOUNT)
            printf("-┐\n");
        else
            printf("--");
    }

    for (int y = 0; y < Y_AMOUNT; y++) {
        /* Draws the left border. */
        printf(" |");
        for (int x = 0; x < X_AMOUNT; x++) {
            struct Cell *target = &board[x][y];

            /* Draw 'Q' if cell is a mine */
            if (target->IsMine && target->IsRevealed)
                printf("%sQ\033[0m", is_selected(target->IsSelected));

            else if (target->IsFlagged)
                printf("\033[0;%dm%sF\033[0m", ColourCodes[8][0], is_selected(target->IsSelected));

            else if (target->IsRevealed) {
                if (target->NeighbourMines > 0) {
                    /* 'Amount' is the amount of NeighbourMines
                     *  off by 1 to access 1st index in matrix. */
                    int Amount = target->NeighbourMines - 1;

                    /* If the colour is dark, then add 'dim' code to printf. */
                    if (ColourCodes[Amount][1])
                        printf("\033[0;%dm\033[2m%s%d\033[0m", ColourCodes[Amount][0], is_selected(target->IsSelected), Amount+1);
                    /* Same but doesn't add dim character */
                    else if (!ColourCodes[Amount][1])
                        printf("\033[0;%dm%s%d\033[0m", ColourCodes[Amount][0], is_selected(target->IsSelected), Amount+1);

                } else
                    printf("%s \033[0m", is_selected(target->IsSelected));
            
            /* Draw X if cell is not revealed */
            } else
                printf("%sX\033[0m", is_selected(target->IsSelected));
                            
            /* Draws spaces between cells and finally the border to the right. */
            if (x+1 != X_AMOUNT)
                printf(" ");
            else
                printf("|");

        }
        printf("\n");
    }
    /* Draws bottom border. */
    printf(" └");
    for (int x = 0; x < X_AMOUNT; x++) {
        if (x+1 == X_AMOUNT)
            printf("-┘");
        else
            printf("--");
    }
    
    /* Draws X cords under the bottom border. */
    /*
    printf("\n  ");
    for (int x = 0; x < X_AMOUNT; x++)
        printf("%d ", x+1);
    */
    
    printf("\n\n");
}


void reveal_empty_cells(int X, int Y)
{
    struct Cell *Target = &board[X][Y];

    if ( X < 0 || X >= X_AMOUNT  ||
         Y < 0 || Y >= Y_AMOUNT  ||
         Target->IsRevealed  || Target->IsMine )
            return;

    if (Target->IsFlagged)
        return;

    Target->IsRevealed = true;

    /* Halt recursion if cell isn't empty. */
    if (Target->NeighbourMines > 0)
        return;

    /* Recursive function to reveal empty cells. */
    for (int offsetY = -1; offsetY <= 1; offsetY++) {
        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            reveal_empty_cells(X+offsetX, Y+offsetY);
        }
    }
}

enum Status check_if_won()
{
    int FlaggedMines  = 0;
    int RevealedCells = 0;
    for (int x = 0; x < X_AMOUNT; x++) {
        for (int y = 0; y < Y_AMOUNT; y++) {
            struct Cell *Target = &board[x][y];

            if (Target->IsMine && Target->IsFlagged)
                FlaggedMines++;

            if (Target->IsRevealed)
                RevealedCells++;
        }
    }

    if (FlaggedMines == MINES && RevealedCells == X_AMOUNT * Y_AMOUNT - MINES)
        return WON;

    return RUNNING;
}

bool reveal_cell(int X, int Y)
{
    struct Cell *Target = &board[X][Y];

    if (Target->IsFlagged)
        return false;

    /* Return 1 if Mine has been hit. */
    if (Target->IsMine) {
        Target->IsRevealed = true;
        return true;
    }

    if (Target->NeighbourMines == 0)
        reveal_empty_cells(X, Y);

    Target->IsRevealed = true;

    return false;
}


void flag_cell(int X, int Y)
{
    struct Cell *Target = &board[X][Y];

    if (Target->IsRevealed)
        return;

    Target->IsFlagged = !Target->IsFlagged;

}


void toggle_cell_as_selected(bool isSelected, int X, int Y)
{
    struct Cell *Target = &board[X][Y];

    Target->IsSelected = isSelected;
}