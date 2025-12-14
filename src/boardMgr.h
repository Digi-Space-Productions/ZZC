/*

    Manages the boards.

*/
#ifndef _BRDMGR_H_
#define _BRDMGR_H_


void changeBoard(ZZTworld *world, uint8_t bn);
void scanForEntities_updateEntites(ZZTworld *world);
void movePlayer();

#endif