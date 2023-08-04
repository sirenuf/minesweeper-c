#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define X_AMOUNT 9
#define Y_AMOUNT 9
#define MINES    10

struct Cell
{
    bool IsMine;
    bool IsRevealed;
    int  NeighbourMines;
};

struct Cell board[X_AMOUNT][Y_AMOUNT];

void init_board(void)
{
    /* Random number seed. */
    srand(time(0));

    int MinesToSet = MINES;
    while (MinesToSet > 0) {
        // Generate random mine location
        int x = rand() % X_AMOUNT;
        int y = rand() % Y_AMOUNT;

        struct Cell *target = &board[x][y];
        
        if (target->IsMine)
            continue;
        
        target->IsMine = true;
        MinesToSet--;
        
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

            if (target->IsMine) //&& target->IsRevealed)
                printf("Q");
            else if (target->IsRevealed)
                printf("-");
            else
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


void reveal_cell(int X, int Y)
{
    struct Cell *target = &board[X][Y];

    target->IsRevealed = true;
}


int main(void)
{
    // Make cell struct in main and pass
    // to draw_board.
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