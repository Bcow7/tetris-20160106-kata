#include "keyboard.h"

#ifndef __ROUND__H
#define __ROUND__H

typedef enum {
	FALLING,
	FROZEN,
	DEAD
} PieceStatus;

void ROUND_setVerticalPause(unsigned char pause);
void ROUND_start();
PieceStatus ROUND_iterate(Arrow arrow);

#ifdef TEST
int testRound();
#endif

#endif
