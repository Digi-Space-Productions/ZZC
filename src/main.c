#include "include.h"
uint8_t currentBoard = 0;
uint8_t oldBoard = 0;
uint8_t firstDraw = 0;
uint8_t gameRunning = 1;
uint8_t whatCharSet = 0;

void clear() {
    #ifdef _WINDOWS
        if (!firstDraw) {
            system("cls");
            firstDraw = 1;
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD home = {0, 0};
        SetConsoleCursorPosition(hConsole, home);
    #else
        system("clear");
    #endif
}

ZZTworld *game;
void renderBoard(uint8_t full) {
    clear();
    printboard(game, full);
}

void gameLoop();
int main() {
    srand(time(NULL)); 
    hideCursor();
    clear();
    printf("-= ZZC =-\nLoading TOWN.ZZT...\n");
    game = zztWorldLoad("TOWN.ZZT");
    changeBoard(game, 0);
    renderBoard(1);
    while (gameRunning) {
        gameLoop();
    }

    firstDraw = 0;
    clear();
    printf("tawa pona!\n");
    showCursor();
}

int ch;
ZZTboard *currentBoardReal;

void initPlayer();
void gameLoop() {
    ch = scanKeyboard();
    currentBoardReal = &game->boards[currentBoard];
    scanForEntities_updateEntites(game);

    if (ch != -1) {                         /* Key exists */
        if (currentBoard == 0) {
            switch(ch) {                        /* Loop for MENU */
                case(KEY_P):                    /* [P] */
                    currentBoard = 1;           /* Board is the first one, so when P is pressed, goto first game scene. */
                    initPlayer();
                break;
            } 
        } else {
            if (playerHealth >= 1) {
                switch(ch) {                        /* Loop for GAME */
                    case(KEY_W):
                        movePlayer(game, 0, -1);
                    break;
                    case(KEY_S):
                        movePlayer(game, 0,  1);
                    break;
                    case(KEY_A):
                        movePlayer(game, -1, 0);
                    break;
                    case(KEY_D):
                        movePlayer(game,  1, 0);
                    break;
                } 
            } else {
                playerHealth = 0;
            }
        }

        switch(ch) {  
            case(KEY_ESC): {
                gameRunning = 0;
            }
        }
    }

    if (currentBoard != oldBoard) {
        oldBoard = currentBoard;
        changeBoard(game, currentBoard);
        renderBoard(1);
    }

    renderBoard(0);
    setCursor(0, ZZT_BOARD_Y_SIZE);
    printf("-= ZZC Client =-\nHealth: %4d | Torches: %4d\nAmmo:   %4d | Gems:    %4d\nScore:  %4d\n", playerHealth, playerTorches, playerAmmo, playerGems, playerScore);
    printf("char = %d\n", ch);

    sleep_(120);
}

void initPlayer() {
    playerHealth   = 100;
    playerAmmo     = 0;
    playerTorches  = 0;
    playerGems     = 0;
    playerScore    = 0;

    playerHas_Key_Blue      = false;
    playerHas_Key_Green     = false;
    playerHas_Key_Cyan      = false;
    playerHas_Key_Red       = false;
    playerHas_Key_Purple    = false;
    playerHas_Key_Yellow    = false;
    playerHas_Key_White     = false;
}