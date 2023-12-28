#include <Windows.h>

void clrscr()
{
    /*
     * CBA
     * https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
     */

    HANDLE hStdOut;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Fetch existing console mode so we correctly add a flag and not turn off others
    DWORD mode = 0;
    GetConsoleMode(hStdOut, &mode);

    // Hold original mode to restore on exit to be cooperative with other command-line apps.
    const DWORD originalMode = mode;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    // Try to set the mode.
    SetConsoleMode(hStdOut, mode);

    // Write the sequence for clearing the display.
    DWORD written = 0;
    PCWSTR sequence = L"\x1b[2J";
    WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL);

    // To also clear the scroll back, emit L"\x1b[3J" as well.
    // 2J only clears the visible window and 3J only clears the scroll back.

    // Restore the mode on the way out to be nice to other command-line applications.
    SetConsoleMode(hStdOut, originalMode);
}