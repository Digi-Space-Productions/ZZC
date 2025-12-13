/*

    Windows Input

*/

#include "include.h"

int scanKeyboard() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir;
    DWORD count;

    while (PeekConsoleInput(hStdin, &ir, 1, &count) && count > 0) {
        ReadConsoleInput(hStdin, &ir, 1, &count);
        if (ir.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD ker = ir.Event.KeyEvent;
            if (ker.bKeyDown) {
                return ker.wVirtualKeyCode;  // VK code (e.g. VK_P = 80)
            }
        }
    }
    return -1; // no key pressed
}