/*

    ZZC Windows Renderer

*/

#include <windows.h>
#include "../include.h"

/* Renders a single colored character. */
void renderChar(int x, int y, char c, unsigned char fg, unsigned char bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD pos;
    DWORD written;
    WORD attrib = 0;

    /* Is in bounds? */
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    if (x < 0 || y < 0 || x >= csbi.dwSize.X || y >= csbi.dwSize.Y) return;

    /* assign X and Y */
    pos.X = (SHORT)x;
    pos.Y = (SHORT)y;

    /* Get color */
    if (fg & 0x1) attrib |= FOREGROUND_BLUE;
    if (fg & 0x2) attrib |= FOREGROUND_GREEN;
    if (fg & 0x4) attrib |= FOREGROUND_RED;
    if (fg & 0x8) attrib |= FOREGROUND_INTENSITY;
    if (bg & 0x1) attrib |= BACKGROUND_BLUE;
    if (bg & 0x2) attrib |= BACKGROUND_GREEN;
    if (bg & 0x4) attrib |= BACKGROUND_RED;
    if (bg & 0x8) attrib |= BACKGROUND_INTENSITY;

    /* Write char + attr. */
    WriteConsoleOutputCharacter(hConsole, &c, 1, pos, &written);
    WriteConsoleOutputAttribute(hConsole, &attrib, 1, pos, &written);
}

void setCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD home;
          home.X = x;
          home.Y = y;
    SetConsoleCursorPosition(hConsole, home);
}

void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;   // hide
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void showCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = TRUE;    // show
    SetConsoleCursorInfo(hOut, &cursorInfo);
}