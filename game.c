#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

const int X_AMOUNT = 8;
const int Y_AMOUNT = 8;

void drawMapToScreen()
{
    /*
    * Draws the map to the screen.
    */

    system("clear");

    /* Y loop keeps redrawing X field with a new line. */
    for (int y = 0; y < Y_AMOUNT; y++) {
        /* Draw the actual characters on x loop. */
        printf("%d|", y);
        for (int x = 0; x < X_AMOUNT; x++) {
            if (x+1 == X_AMOUNT)
                printf("X");
            else
                printf("X ");
        }

        printf("|\n");
    }

    // Prints wall between before X cords.
    printf("  ");
    for (int x = 0; x < X_AMOUNT; x++) {
        if (x+1 == X_AMOUNT)
            printf("-");
        else
            printf("--");
    }

    // Prints X cords
    printf("\n  ");
    for (int x = 0; x < X_AMOUNT; x++)
        printf("%d ", x);

    printf("\n\n");
}

int main(void)
{
    int Running = true;

    while(Running)
    {
        drawMapToScreen();
        scanf("%d", &Running);
    }

    printf("\n");
    return 0;
}