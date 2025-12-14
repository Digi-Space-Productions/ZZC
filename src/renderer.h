#ifndef _RENDERER_H
#define _RENDERER_H

void renderChar(int x, int y, char c, unsigned char fg, unsigned char bg);
void setCursor(int x, int y);
void hideCursor();
void showCursor();
void sleep_();

#endif