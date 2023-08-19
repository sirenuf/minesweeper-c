#include <stdio.h>
#include <stdbool.h>

#include "input.h"
#include "game.h"

int main(void)
{
    /* TODO:
     * Make cell struct in main
     * and pass to draw_board.
     */
    while (true) {
        int PlaceX = 0;
        int PlaceY = 0;
        int Status = RUNNING;

        init_board();
        while (Status == RUNNING) {
            toggle_cell_as_selected(true, PlaceX, PlaceY);
            draw_board();
            toggle_cell_as_selected(false, PlaceX, PlaceY);
            switch (handle_input()) {
                case QUIT:
                    Status = EXIT;
                    break;
                case FLAG:
                    flag_cell(PlaceX, PlaceY);
                    break;
                case REVEAL:
                    bool Result = reveal_cell(PlaceX, PlaceY);
                    if (Result)
                        Status = LOST;

                    break;
                case UP:
                    PlaceY--;
                    break;
                case DOWN:
                    PlaceY++;
                    break;
                case RIGHT:
                    PlaceX++;
                    break;
                case LEFT:
                    PlaceX--;
                    break;
            }

            /* Bounces cursor. */
            if (PlaceX < 0)
                PlaceX = X_AMOUNT - 1;
            else if (PlaceX >= X_AMOUNT)
                PlaceX = 0;
            else if (PlaceY < 0)
                PlaceY = Y_AMOUNT - 1;
            else if (PlaceY >= Y_AMOUNT)
                PlaceY = 0;
            
            if (Status == RUNNING)
                Status = check_if_won();
        }

        if (Status == WON) {
            draw_board();
            printf("Congratulations! You won!\n");
        }
        
        if (Status == LOST) {
            draw_board();
            printf("You hit a mine! You lost.\n");
        }

        if (Status == EXIT)
            return 0;



        if (Status == WON || Status == LOST) {
            int Answer;
            printf("Do you want to play again? (y/n)\n");
            
            while (true) {
                printf(">");
                Answer = getchar();
                while(getchar() != '\n'); // Clear input.
                if (Answer == 'y' || Answer == 'Y')
                    break;
                else if (Answer == 'n' || Answer == 'N')
                    return 0;
            }
        }
    }

    return 0;
}