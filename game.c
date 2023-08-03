#include <stdio.h>
#include <stdbool.h>

#define X_AMOUNT 8
#define Y_AMOUNT 8

struct Cell
{
    bool isMine;
    bool isRevealed;
    int  neighbourMines;
};

struct Cell board[X_AMOUNT][Y_AMOUNT];

void init_board(void)
{
    for (int x = 0; x < X_AMOUNT; x++) {
        for (int y = 0; y < Y_AMOUNT; y++) {
            struct Cell *target = &board[x][y];

            target->isMine = false;
            target->isRevealed = false;
            //target->neighbourMines = 0; To be made
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

            if (target->isMine) {
                printf("It's a mine!\n"); // Soon make the game exit.
                return;
            } else if (target->isRevealed) {
                target->isRevealed = true;
                printf("-");
            } else {
                printf("X");
            }
            
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
    printf("X: %d\nY: %d", X, Y);
    target->isRevealed = true;
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