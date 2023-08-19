#include <unistd.h>
#include <termios.h>
#include <stdbool.h>

enum Commands {
    QUIT = -1,
    FLAG,
    REVEAL,
    UP,
    DOWN,
    RIGHT,
    LEFT
};

void toggle_canonical(bool Toggle)
{
    struct termios Attr;

    tcgetattr(STDIN_FILENO, &Attr);

    if (Toggle)      
        Attr.c_lflag |= (ICANON | ECHO);  // Enables canonical input
    else if(!Toggle) 
        Attr.c_lflag &= ~(ICANON | ECHO); // Disables canonical input

    tcsetattr(STDIN_FILENO, TCSANOW, &Attr);
}

enum Commands handle_input(void)
{
    /* Disable canonical input. */
    toggle_canonical(false);

    char Buffer;
    int Result = -2;

    /*
     * Keep listening for new keypress.
     * 
     * 3rd argument resembles amount of bytes to read.
     * If read() returns the same value as the 3rd argument,
     * then it has succeeded.
     */
    while (read(STDIN_FILENO, &Buffer, 1 && Result == -2) == 1) {
        /*
         * Constantly recheck buffer.
         * If they equal the same start sequence as
         * Arrow keys, then proceede to switch
         */
        if (Buffer == 27 && read(STDIN_FILENO, &Buffer, 1) == 1 &&
            Buffer == 91 && read(STDIN_FILENO, &Buffer, 1) == 1 && 
            Buffer >= 65 && Buffer <= 68 )
            {   /* E.g: 65 - 63 = 2. */
                Result = Buffer - 63;
            }
        else if (Buffer == 'f' || Buffer == 'F')
            Result = FLAG;
        else if (Buffer == 'q' || Buffer == 'Q')
            Result = QUIT;
        else if (Buffer == 32) // Spacebar
            Result = REVEAL;
    }
    toggle_canonical(true);
    return Result;

}