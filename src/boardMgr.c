/*

    Manages the boards.

*/
#include "include.h"

/* OLD BOARD, important failsafe. */
ZZTboard *OLDBOARD;

void changeBoard(ZZTworld *world, uint8_t bn) {
    world->cur_board = bn;
    currentBoard = bn;
    zztBoardDecompress(&(world->boards[bn]));
}

int pickupObject(ZZTblock *b, ZZTtile *tile, int type, int x, int y);
uint8_t slowCMP(ZZTblock *b, ZZTtile *tile, int x, int y) {
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
            return pickupObject(b, tile, type, x, y);
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
        if (slowCMP(b, &tx, pxC, py)) {
            crtB->plx += x_offs;
        }
        if (slowCMP(b, &ty, px, pyC)) {
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