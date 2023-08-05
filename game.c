#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define X_AMOUNT 9
#define Y_AMOUNT 6
#define MINES    10

struct Cell
{
    bool IsMine;
    bool IsRevealed;
    int  NeighbourMines;
};

struct Cell board[X_AMOUNT][Y_AMOUNT];

/* Store colour codes */
const int ColourCodes[8][2] = { 
    { 34, false }, // Blue
    { 32, false }, // Green
    { 31, false }, // Red
    { 34, true  }, // Dark blue
    { 31, true  }, // Dark red
    { 36, false }, // Cyan
    { 37, true  }, // Black (dark white)
    { 37, false }  // Grey  (white)
};

void init_board(void)
{
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
                         x+offsetX >= 0 && y+offsetX < X_AMOUNT ) {
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
                printf("Q");

            else if (target->IsRevealed) {
                if (target->NeighbourMines > 0) {
                    /* 'Amount' is the amount of NeighbourMines
                     *  off by 1 to access 1st index in matrix. */
                    int Amount = target->NeighbourMines - 1;

                    /* If the colour is dark, then add 'dim' code to printf. */
                    if (ColourCodes[Amount][1])
                        printf("\033[0;%dm\033[2m%d\033[0m", ColourCodes[Amount][0], Amount+1);
                    /* Same but doesn't add dim character */
                    else if (!ColourCodes[Amount][1])
                        printf("\033[0;%dm%d\033[0m", ColourCodes[Amount][0], Amount+1);

                } else
                    printf(" ");
            
            /* Draw X if cell is not revealed */
            } else
                printf("X");
            
            /* Draws the spaces between X characters and the right border. */
            if (x+1 != X_AMOUNT)
                printf(" ");
            else
                printf("|%d", y+1);

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
    printf("\n  ");
    for (int x = 0; x < X_AMOUNT; x++)
        printf("%d ", x+1);

}


void reveal_empty_cells(int X, int Y)
{
    if ( X < 0 || X > X_AMOUNT  ||
         Y < 0 || Y > Y_AMOUNT  ||
         board[X][Y].IsRevealed || board[X][Y].IsMine )
            return;

    board[X][Y].IsRevealed = true;

    /* Halt recursion if cell isn't empty. */
    if (!board[X][Y].NeighbourMines == 0)
        return;

    /* Recursive function to reveal empty cells. */
    for (int offsetY = -1; offsetY <= 1; offsetY++) {
        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            reveal_empty_cells(X+offsetX, Y+offsetY);
        }
    }
}


void reveal_cell(int X, int Y)
{
    struct Cell *target = &board[X][Y];

    if (target->NeighbourMines == 0) {
        reveal_empty_cells(X, Y);
    }

    target->IsRevealed = true;
}


int main(void)
{
    /* TODO:
     * Make cell struct in main
     * and pass to draw_board.
     */
    int PlaceX;
    int PlaceY;
    bool Running = true;

    init_board();

    while (Running) {
        draw_board();

        printf("\n>");
        scanf("%d %d", &PlaceX, &PlaceY);
        PlaceX--;
        PlaceY--;
        reveal_cell(PlaceX, PlaceY);
        printf("\n");
    }

    return 0;
}