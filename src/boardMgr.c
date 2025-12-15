/*

    Manages the boards.

*/
#include "include.h"

/* OLD BOARD, important failsafe. */
ZZTboard *OLDBOARD;
ZZTboard *currentBoard_global;

static int clcDst(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1; if (dx < 0) dx = -dx;
    int dy = y2 - y1; if (dy < 0) dy = -dy;

    int mn = dx < dy ? dx : dy;
    return dx + dy - (mn >> 1) - (mn >> 2);     /* small error */
}
static int bestDir(int x, int y, int tx, int ty) {
    int best = 3;
    int bestDst = clcDst(x, y + 1, tx, ty);     /* North*/

    int d = clcDst(x + 1, y, tx, ty);
    if (d < bestDst) { bestDst = d; best = 1; } /* Shouth */

    d = clcDst(x, y - 1, tx, ty);
    if (d < bestDst) { bestDst = d; best = 0; } /* East */

    d = clcDst(x - 1, y, tx, ty);
    if (d < bestDst) { bestDst = d; best = 2; } /* West */

    return best;
}

void scanForEntities_updateEntites(ZZTworld *world);
void changeBoard(ZZTworld *world, uint8_t bn) {
    world->cur_board = bn;
    currentBoard = bn;
    zztBoardDecompress(&(world->boards[bn]));
}

int pickupObject(ZZTblock *b, ZZTtile *tile, int type, int x, int y);
uint8_t checkCollision(ZZTblock *b, ZZTtile *tile, int x, int y, uint8_t isPlayer) {
    int type = tile->type;
    switch(type) {
        case(ZZT_EMPTY):
        case(ZZT_FAKE):
        case(ZZT_PLAYER):
            return 1;
        break;

        /* Items/Item related */
        case(ZZT_TORCH):
        case(ZZT_AMMO):
        case(ZZT_GEM):
        case(ZZT_FOREST):
        case(ZZT_DOOR):
        case(ZZT_KEY):
        /* Entities */
        case(ZZT_LION):
        case(ZZT_TIGER):
        case(ZZT_CENTHEAD):
        case(ZZT_CENTBODY):
        case(ZZT_SHARK):
        case(ZZT_SLIME):
        case(ZZT_BEAR):
            if (isPlayer) {
                return pickupObject(b, tile, type, x, y);
            }
        break;
    }
    return 0;
}

/* Player pickup function*/
int pickupObject(ZZTblock *b, ZZTtile *tile, int type, int x, int y) {
    uint8_t canBreak = 1;

    switch(type) {
        case(ZZT_TORCH): {
            playerTorches++;
            break;
        };
        case(ZZT_AMMO): {
            playerAmmo+=5;
            break;
        };
        case(ZZT_GEM): {
            playerGems++;
            playerHealth++;
            playerScore+=10;
            break;
        };
        case(ZZT_KEY): {
            uint8_t clrD = tile->color;
            uint8_t low = clrD & 0x0F;
            uint8_t high = (clrD >> 4) & 0x0F;

            /* Bodgy if/else statement, will make a switch in the future, im just too fucking lazy */
            /* Yandere  dev momento */
            if ((low == 0x9) && !playerHas_Key_Blue) {
                playerHas_Key_Blue = true;

            } else if ((low == 0xA) && !playerHas_Key_Green) {
                playerHas_Key_Green = true;

            } else if ((low == 0xB) && !playerHas_Key_Cyan) {
                playerHas_Key_Cyan = true;

            } else if ((low == 0xC) && !playerHas_Key_Red) {
                playerHas_Key_Red = true;

            } else if ((low == 0xD) && !playerHas_Key_Purple) {
                playerHas_Key_Purple = true;

            } else if ((low == 0xE) && !playerHas_Key_Yellow) {
                playerHas_Key_Yellow = true;

            } else if ((low == 0xF) && !playerHas_Key_White) {
                playerHas_Key_White = true;
            } else {
                canBreak = 0;
            }
            break;
        };
        case(ZZT_DOOR): {
            uint8_t clrD = tile->color;
            uint8_t low = clrD & 0x0F;
            uint8_t high = (clrD >> 4) & 0x0F;

            /* Bodgy if/else statement, will make a switch in the future, im just too fucking lazy */
            /* Yandere  dev momento */
            if (playerHas_Key_Blue && (high == 0x1)) {
                playerHas_Key_Blue = false;

            } else if (playerHas_Key_Green && (high == 0x2)) {
                playerHas_Key_Green = false;

            } else if (playerHas_Key_Cyan && (high == 0x3)) {
                playerHas_Key_Cyan = false;

            } else if (playerHas_Key_Red && (high == 0x4)) {
                playerHas_Key_Red = false;

            } else if (playerHas_Key_Purple && (high == 0x5)) {
                playerHas_Key_Purple = false;

            } else if (playerHas_Key_Yellow && (high == 0x6)) {
                playerHas_Key_Yellow = false;

            } else if (playerHas_Key_White && (high == 0x7)) {
                playerHas_Key_White = false;

            } else {
                canBreak = 0;
            }
            break;
        }
        case(ZZT_LION):
        case(ZZT_TIGER):
        case(ZZT_CENTHEAD):
        case(ZZT_CENTBODY):
        case(ZZT_SHARK):
        case(ZZT_SLIME):
        case(ZZT_BEAR): {
            playerHealth -= 10;
            break;
        };
    }

    if (canBreak) {
        tile->type = ZZT_EMPTY;
        zztTileSet(b, x, y, *tile);
    }

    return canBreak;
}

//zztTileSet(b, x, y, t)

void border(ZZTworld *world, int x, int y);
int ox, oy;
void movePlayer(ZZTworld *world, int x_offs, int y_offs) {
    ZZTboard *crtB = zztBoardGetCurPtr(world);
    ZZTblock *b = crtB->bigboard;
    int px = crtB->plx;
    int py = crtB->ply;
    int pxC= crtB->plx+x_offs;
    int pyC= crtB->ply+y_offs;


    /* Bodgy border logic */
    ox = crtB->plx;
    oy = crtB->ply;
    /* X */
        if (pxC == -1) {
            crtB->plx = 0;
            ox = ZZT_BOARD_X_SIZE-1;
            border(world, -1, 0);
        }
        if (pxC == ZZT_BOARD_X_SIZE) {
            crtB->plx = ZZT_BOARD_X_SIZE-1;
            ox = 0;
            border(world, 1, 0);
        }
        /* Y */
        if (pyC == -1) {
            crtB->ply = 0;
            oy = ZZT_BOARD_Y_SIZE-1;
            border(world, 0, 1);
        }
        if (pyC == ZZT_BOARD_Y_SIZE) {
            crtB->ply = ZZT_BOARD_Y_SIZE-1;
            oy = 0;
            border(world, 0, -1);
        }
    
    /* Collision */
    ZZTtile tx = zztTileAt(b, pxC, py);
    ZZTtile ty = zztTileAt(b, px, pyC);
        if (checkCollision(b, &tx, pxC, py, 1)) {
            crtB->plx += x_offs;
        }
        if (checkCollision(b, &ty, px, pyC, 1)) {
            crtB->ply += y_offs;
        }
}

/* (Currently bodgy) function that moves the board dependant on the X and Y offset. */
void border(ZZTworld *world, int x, int y) {
    ZZTboard *crtB = zztBoardGetCurPtr(world);

    if (x == 1) {
        changeBoard(world, crtB->info.board_e);
    }
    if (x == -1) {
        changeBoard(world, crtB->info.board_w);
    }

    if (y == 1) {
        changeBoard(world, crtB->info.board_n);
    }
    if (y == -1) {
        changeBoard(world, crtB->info.board_s);
    }

    crtB = zztBoardGetCurPtr(world);
    crtB->plx = ox;
    crtB->ply = oy;
}

/* Entity */
int8_t testAAA = 1;
int8_t testBBB = 0;
uint8_t ignore[ZZT_BOARD_X_SIZE*ZZT_BOARD_Y_SIZE];
uint8_t checkIgnoreList(int x, int y);
void markIgnored(int x, int y);
void pushObject(ZZTworld *world, int x, int y, int nX, int nY);
uint16_t cycle = 0;
void scanForEntities_updateEntites(ZZTworld *world) {
    ZZTboard *crtB = zztBoardGetCurPtr(world);
    ZZTblock *b = crtB->bigboard;

    memset(ignore, 0, sizeof(ignore));
    int8_t movOfsX = 0;
    int8_t movOfsY = 0;

    for (int yy = 0; yy < ZZT_BOARD_Y_SIZE; yy++) {
        for (int xx = 0; xx < ZZT_BOARD_X_SIZE; xx++) {
            ZZTtile tile = zztTileAt(b, xx, yy);
            uint8_t rnd = rand() % 5;
            if (rnd == 4) {
                rnd = bestDir(xx, yy, crtB->plx, crtB->ply);
            }
            
            switch (rnd) {
                case(0):
                    movOfsX = 1;
                    movOfsY = 0;
                break;
                case(1):
                    movOfsX = 0;
                    movOfsY = 1;
                break;
                case(2):
                    movOfsX = -1;
                    movOfsY = 0;
                break;
                case(3):
                    movOfsX = 0;
                    movOfsY = -1;
                break;
            }

            if (((tile.type == ZZT_LION) || (tile.type == ZZT_TIGER)) && !(checkIgnoreList(xx,yy)) && ((cycle%2) == 0)) {
                ZZTtile tileS = zztTileAt(b,xx+movOfsX,yy+movOfsY);
                uint8_t testS = checkCollision(b, &tileS, xx+movOfsX, yy+movOfsY, 0);
                if (testS) {
                    pushObject(world, xx, yy, xx+movOfsX,yy+movOfsY);
                }
            }
        }
    }

    testAAA += testBBB;
    if (testAAA > 3) {
        testBBB = -1;
    }
    if (testAAA < -3) {
        testBBB = 1;
    }
    cycle++;
    cycle%=1000;
}

void pushObject(ZZTworld *world, int x, int y, int nX, int nY) {
    ZZTboard *crtB = zztBoardGetCurPtr(world);
    ZZTblock *b = crtB->bigboard;

    ZZTtile tileOriginal = zztTileAt(b,x,y);
    ZZTtile tileNew      = zztTileAt(b,nX,nY);

    tileNew.type = tileOriginal.type;
    tileNew.color = tileOriginal.color;
    tileOriginal.type = ZZT_EMPTY;
    zztTileSet(b, x, y,  tileOriginal);
    zztTileSet(b, nX,nY, tileNew);

    markIgnored(x, y);
    markIgnored(nX,nY);
}

uint8_t checkIgnoreList(int x, int y) {
    return ignore[((y)*ZZT_BOARD_X_SIZE) + (x)];
}

void markIgnored(int x, int y) {
    ignore[((y)*ZZT_BOARD_X_SIZE) + (x)] = 1;
}