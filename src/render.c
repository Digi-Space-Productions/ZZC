#include "include.h"

ZZTboard *currentBrd;

/* Shadow buffer to remember last drawn state */
typedef struct {
    char ch;
    unsigned char fg;
    unsigned char bg;
} Cell;

static Cell lastFrame[ZZT_BOARD_Y_SIZE][ZZT_BOARD_X_SIZE];
static char lastMsg[ZZT_MESSAGE_SIZE];

void initFrameBuffer() {
    for (int y = 0; y < ZZT_BOARD_Y_SIZE; y++) {
        for (int x = 0; x < ZZT_BOARD_X_SIZE; x++) {
            lastFrame[y][x].ch = 0;
            lastFrame[y][x].fg = 0xFF;
            lastFrame[y][x].bg = 0xFF;
        }
    }
    memset(lastMsg, 0, sizeof(lastMsg));
}

void printblock(ZZTblock * b, int fullRedraw) {
    int x, y;
    for (y = 0; y < b->height; y++) {
        for (x = 0; x < b->width; x++) {
            unsigned char cl = zztTileGetDisplayColor(b, x, y);
            char c  = zztTileGetDisplayChar(b, x, y);

            /* Get the current tile */
            ZZTtile tile = zztTileAt(b, x, y);
            if ((tile.type == ZZT_PLAYER)/*&(currentBoard==0)*/) { c = ' '; cl = 0x00; }    /* If player and on board 0, then that means they shouldn't render. */
                                                                                            /* I am unsure if this is how the actual logic works. */
                                                                                            
            if (((x == currentBrd->plx) && (y == currentBrd->ply)) && (currentBoard!=0)) { c = 0x02; cl = 0x1F; }
            if (c == '\n') c = ' ';

            unsigned char fg = cl & 0x0F;
            unsigned char bg = (cl >> 4) & 0x0F;

            c = tolower(onlyASCII[c]);

            /* Only redraw if different from last frame */
            if (fullRedraw ||
                lastFrame[y][x].ch != c ||
                lastFrame[y][x].fg != fg ||
                lastFrame[y][x].bg != bg) {
                renderChar(x, y, c, fg, bg);
                lastFrame[y][x].ch = c;
                lastFrame[y][x].fg = fg;
                lastFrame[y][x].bg = bg;
            }
        }
    }

    /* Only redraw message if it changed */
    if (fullRedraw ||
        memcmp(lastMsg, currentBrd->info.message, ZZT_MESSAGE_SIZE) != 0) {
        printf("%.*s\n", ZZT_MESSAGE_SIZE, currentBrd->info.message);
        memcpy(lastMsg, currentBrd->info.message, ZZT_MESSAGE_SIZE);
    }
}

void printboard(ZZTworld * w, int fullRedraw) {
    currentBrd = zztBoardGetCurPtr(w);
    printblock(currentBrd->bigboard, fullRedraw);
    setCursor(0, ZZT_BOARD_Y_SIZE);
}
