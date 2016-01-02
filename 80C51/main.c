#include <mcs51reg.h>
#include "stdio-t6963c.h"
#include "test.h"
#include "keyboard.h"
#include "board.h"
#include "piece.h"
#include "round.h"

// Tetris-0

#ifndef TEST

void pause(unsigned int t) {
	unsigned int n;
	for (n = 0; n < t; n++);
}

void initialize() {
	STDIO_initialize();
	BOARD_initialize();
	ROUND_setVerticalPause(200);
}

PieceStatus round() {
	PieceStatus status;
	Arrow arrow;

	ROUND_start();

	do {
		arrow = KEYBOARD_keyPress();
		status = ROUND_iterate(arrow);
		pause(200);
	} while (status == FALLING);

	return status;
}

void play() {
	while(round() != DEAD) {
		BOARD_clearSolidRows();
	}
	BOARD_display(6, 7, " Vous avez perdu ");
}

void main(void) {
	initialize();
	play();

	while(1);
}

#else
#include <stdio.h>
void main(void) {
	int testsInError = 0;
	STDIO_initialize();

	testsInError += testKeyboard();
	testsInError += testBoard();
	testsInError += testPiece();
	testsInError += testRound();

	printf("%d tests en erreur", testsInError);

	while(1);
}
#endif