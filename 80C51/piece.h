#include "main.h"

#ifndef __PIECE__H
#define __PIECE__H

typedef struct {
	Block block;
	Position position;
	unsigned char orientation;
} Piece;

typedef enum {
	I,
	O,
	T,
	S,
	Z,
	J,
	L, 
#ifdef TEST
	ABCF,
	BLOCKING
#endif
} TetrominoType;

void PIECE_initialize(Piece *piece, TetrominoType type, unsigned char x, unsigned char y, unsigned char orientation);
void PIECE_place(Piece *piece);
void PIECE_clear(Piece *piece);
char PIECE_canPlace(Piece *piece);
void PIECE_freeze(Piece *piece);

#ifdef TEST
Piece *obtainPieceLShapeAt(char x, char y, unsigned int orientation);
Piece *obtainPieceBlockingAt(char x, char y);

int testPiece();
#endif

#endif