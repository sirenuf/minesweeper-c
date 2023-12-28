#include <Windows.h>
#include <stdbool.h>
#include "../game.h"

void toggle_canonical(bool Enable)
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;

    GetConsoleMode(hStdin, &mode);

    if (Enable)
        mode |=  (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    else if (!Enable)
        mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    
    if (!SetConsoleMode(hStdin, mode)) {
        printf("Error. Error code: %d", GetLastError());
        exit(1);
    }
}

enum Commands handle_input(void)
{
    /* https://learn.microsoft.com/en-us/windows/console/reading-input-buffer-events */
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead, i;
    int short Result;

    toggle_canonical(false);

    while (true) {
        ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

        for (i = 0; i < cNumRead; i++) {
            if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown) {
                switch (irInBuf[i].Event.KeyEvent.wVirtualKeyCode) {
                    case VK_UP:
                        Result = UP;
                        break;

                    case VK_DOWN:
                        Result = DOWN;
                        break;

                    case VK_LEFT:
                        Result = LEFT;
                        break;

                    case VK_RIGHT:
                        Result = RIGHT;
                        break;

                    case 'F':
                        Result = FLAG;
                        break;

                    case 'Q':
                        Result = QUIT;
                        break;
                    
                    case VK_SPACE:
                        Result = REVEAL;
                        break;

                    default:
                        continue;
                }
                toggle_canonical(true);
                return Result;
            }
        }
    }
}