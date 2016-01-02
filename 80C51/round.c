#include <stdlib.h>
#include "round.h"
#include "piece.h"
#include "board.h"
#include "main.h"
#include "keyboard.h"
#include "bdd.h"
#include "test.h"

void modifyPiecePositionAndOrientationAccordingToArrow(Piece *piece, Arrow arrow) {
	switch(arrow) {
		case ARROW_ROTATE_LEFT:
			piece->orientation--;
			break;
		case ARROW_ROTATE_RIGHT:
			piece->orientation++;
			break;
		case ARROW_LEFT:
			piece->position.x--;
			break;
		case ARROW_RIGHT:
			piece->position.x++;
			break;
	}	
}

/**
 * Indique si la pièce, dans sa rotation et position actuelle, peut
 * encore tomber de un cran.
 * @param piece La description de la pièce.
 * @return 1 si la pièce peut encore descendre. 0 autrement.
 */
unsigned char ROUND_pieceCanFall(Piece *piece) {
	// À faire
	return 0;	
}

/**
 * Indique si la pièce peut se déplacer dans la direction
 * de la flèche.
 * @param piece La description de la pièce.
 * @param arrow La direction de mouvement.
 * @return 1 si la pièce peut se déplacer dans cette direction.
 */
unsigned char ROUND_pieceCanMove(Piece *piece, Arrow arrow) {
	// À faire
	return 0;
}

/**
 * Fait tomber la pièce de un niveau, à moins qu'elle ne soit bloquée
 * par le fond du tableau de jeu ou un obstacle.
 * Si la pièce est bloquée et elle ne peut pas tomber, alors elle est figée.
 * @param piece La description de la pièce.
 * @return L'état de la pièce:
 * - FALLING si la pièce n'est pas encore figée.
 * - FROZEN si la pièce ne peut plus tomber.
 */
PieceStatus ROUND_fallOrFreeze(Piece *piece) {
	// À faire
	return FROZEN;
}

/**
 * Dépace (si possible) la pièce selon la flèche indiquée.
 * La pièce se déplace à gauche ou à droite, ou bien tourne sur elle même.
 * Si la pièce est bloquée par un bord ou un obstacle, alors elle reste dans
 * sa position actuelle.
 * @param piece La description de la pièce.
 * @param arrow La direction de mouvement.
 */
void ROUND_move(Piece *piece, Arrow arrow) {
	// À faire
}

// Nombre d'itérations avec la pièce avant qu'elle ne tombe de un niveau.
unsigned char verticalPause;

/**
 * Établit le nombre d'itération avec la pièce avant qu'elle ne tombe de un niveau.
 * @param pause Plus cette valeur est grande, plus la pièce tombe lentement.
 */
void ROUND_setVerticalPause(unsigned char pause) {
	verticalPause = pause;
}

// Compte le nombre d'itérations avec la pièce avant qu'elle ne tombe de un niveau.
unsigned char time = 0;

// Appelée à chaque itération avec la pièce,
// cette fonction indique quand il est temps de faire descendre la pièce
// de 1 niveau.
char timeToFall() {
	if (time-- == 0) {
		time = verticalPause;
		return 1;
	}
	return 0;
}

/**
 * Effectue une itération avec la pièce.
 * - La pièce peut se déplacer à tout moment, selon la flèche indiquée.
 * - Régulièrement, la pièce tombe de un niveau.
 * - Si la pièce ne peut plus tombée, alors elle reste figée.
 * - Si la pièce reste figée au sommet du tableau de jeu, alors la partie est finie.
 * @param piece La description de la pièce.
 * @param arrow La direction de mouvement.
 * @return L'état de la pièce:
 * - FALLING si la pièce n'est pas encore figée.
 * - FROZEN si la pièce ne peut plus tomber.
 * - DEAD si la pièce est figée au sommet du tableau de jeu.
 */
PieceStatus ROUND_iteratePiece(Piece *piece, Arrow arrow) {
	PieceStatus status;

	ROUND_move(piece, arrow);

	if (timeToFall()) {
		status = ROUND_fallOrFreeze(piece);

		if ((status == FROZEN) && piece->position.y == TETRIS_LIMIT_Y0) {
			return DEAD;
		}
		return status;
	}
	return FALLING;
}

/**
 * Initialise une pièce au sommet du tableau de jeu.
 * @param piece La pièce à initialiser.
 */
void ROUND_startPiece(Piece *piece) {
	TetrominoType type = rand() % 7;
	time = verticalPause;
	PIECE_initialize(piece, type, (TETRIS_LIMIT_X1 + TETRIS_LIMIT_X0) / 2, TETRIS_LIMIT_Y0, 0);
}

// Piece active pendant ce round
Piece piece;

/**
 * Initialise une nouvelle pièce.
 */
void ROUND_start() {
	ROUND_startPiece(&piece);
}

/**
 * Exécute une itération.
 */
PieceStatus ROUND_iterate(Arrow arrow) {
	return ROUND_iteratePiece(&piece, arrow);
}

#ifdef TEST

int bddPieceCanFall() {
	int testsInError = 0;
	BddContent c = {
		"cf........",
		"b.........",
		"a%%.......",
		".%%.......",
		"..........",
	};
	Piece *pieceLShape = obtainPieceLShapeAt(0, 0, 3);

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(1, 2));

	testsInError += assertEquals(ROUND_pieceCanFall(pieceLShape), 1, "ROF00");

	PIECE_place(pieceLShape);
	testsInError += BDD_assert(c, "PCF01");

	return testsInError;
}

int bddPieceCanNotFall() {
	int testsInError = 0;
	BddContent c = {
		"cf........",
		"b%%.......",
		"a%%.......",
		"..........",
		"..........",
	};
	Piece *pieceLShape = obtainPieceLShapeAt(0, 0, 3);

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(1, 1));

	testsInError += assertEquals(ROUND_pieceCanFall(pieceLShape), 0, "ROF10");

	PIECE_place(pieceLShape);
	testsInError += BDD_assert(c, "PCF11");

	return testsInError;
}

int bddPieceCanMove() {
	int testsInError = 0;

	BddContent c = {
		"..........",
		".%%.......",
		".%%cf.....",
		"...b......",
		"...a......",
	};

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(1, 1));

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(4, 2, 0), ARROW_LEFT), 1, "ROCML1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(4, 2, 1), ARROW_LEFT), 1, "ROCML2");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(4, 2, 2), ARROW_LEFT), 1, "ROCML3");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(4, 2, 3), ARROW_LEFT), 1, "ROCML4");

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(6, 2, 0), ARROW_RIGHT), 1, "ROCMR1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(6, 2, 1), ARROW_RIGHT), 1, "ROCMR2");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(6, 2, 2), ARROW_RIGHT), 1, "ROCMR3");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(6, 2, 3), ARROW_RIGHT), 1, "ROCMR4");

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 0), ARROW_ROTATE_LEFT), 1, "ROCRL1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 1), ARROW_ROTATE_LEFT), 1, "ROCRL2");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 2), ARROW_ROTATE_LEFT), 1, "ROCRL3");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 3), ARROW_ROTATE_LEFT), 1, "ROCRL4");

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 0), ARROW_ROTATE_RIGHT), 1, "ROCRR1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 1), ARROW_ROTATE_RIGHT), 1, "ROCRR2");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 2), ARROW_ROTATE_RIGHT), 1, "ROCRR3");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 2, 3), ARROW_ROTATE_RIGHT), 1, "ROCRR4");

	PIECE_place(obtainPieceLShapeAt(3, 2, 3));
	testsInError += BDD_assert(c, "ROCM");

	return testsInError;
}

int bddPieceCannotMoveLeft() {
	int testsInError = 0;

	BddContent c = {
		"..........",
		".%%a......",
		".%%b......",
		"..fc......",
		"..........",
	};

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(1, 1));

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(0, 3, 0), ARROW_LEFT), 0, "ROCNML1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(2, 1, 1), ARROW_LEFT), 0, "ROCNML2");

	PIECE_place(obtainPieceLShapeAt(2, 1, 1));
	testsInError += BDD_assert(c, "PCCNML");

	return testsInError;
}

int bddPieceCannotMoveRight() {
	int testsInError = 0;

	BddContent c = {
		"cf........",
		"b%%.......",
		"a%%.......",
		"..........",
		"..........",
	};

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(1, 1));

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(7, 3, 0), ARROW_RIGHT), 0, "ROCNMR1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(0, 0, 3), ARROW_RIGHT), 0, "ROCNMR2");

	PIECE_place(obtainPieceLShapeAt(0, 0, 3));
	testsInError += BDD_assert(c, "ROCNMR");

	return testsInError;
}

int bddPieceCannotRotateLeft() {
	int testsInError = 0;

	BddContent c = {
		".abc......",
		".%%f......",
		".%%.......",
		"..........",
		"..........",
	};

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(1, 1));

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(1, 0, 0), ARROW_ROTATE_LEFT), 0, "ROCNRL1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(3, 3, 0), ARROW_ROTATE_LEFT), 0, "ROCNRL2");

	PIECE_place(obtainPieceLShapeAt(1, 0, 0));
	testsInError += BDD_assert(c, "ROCNRL");

	return testsInError;
}

int bddPieceCannotRotateRight() {
	int testsInError = 0;

	BddContent c = {
		".a%%......",
		".b%%......",
		"fc........",
		"..........",
		"..........",
	};

	BDD_clear();
	PIECE_place(obtainPieceBlockingAt(2, 0));

	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(8, 0, 1), ARROW_ROTATE_RIGHT), 0, "ROCNRR1");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(0, 3, 2), ARROW_ROTATE_RIGHT), 0, "ROCNRR2");
	testsInError += assertEquals(ROUND_pieceCanMove(obtainPieceLShapeAt(0, 0, 1), ARROW_ROTATE_RIGHT), 0, "ROCNRR3");

	PIECE_place(obtainPieceLShapeAt(0, 0, 1));
	testsInError += BDD_assert(c, "ROCNRL");

	return testsInError;
}

int bddTestIterationWithPiece() {
	int testsInError = 0;
	BddContent c = {
		"        ..",
		"        ..",
		"%%     ...",
		"%%%%%.....",
		"%%%%%....."
	};
	Piece *piece;

	BDD_clear();
	ROUND_setVerticalPause(5);

	piece = obtainPieceLShapeAt(5, 0, 0);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_ROTATE_RIGHT);
	ROUND_iteratePiece(piece, ARROW_ROTATE_RIGHT);
	while(ROUND_iteratePiece(piece, ARROW_NEUTRAL) == FALLING);

	piece = obtainPieceLShapeAt(5, 0, 0);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	while(ROUND_iteratePiece(piece, ARROW_NEUTRAL) == FALLING);

	piece = obtainPieceLShapeAt(5, 0, 0);
	ROUND_iteratePiece(piece, ARROW_ROTATE_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	ROUND_iteratePiece(piece, ARROW_LEFT);
	while(ROUND_iteratePiece(piece, ARROW_NEUTRAL) == FALLING);

	testsInError += BDD_assert(c, "ROIT");

	return testsInError;
}

void testIteration() {
	PieceStatus status;

	BDD_clear();

	do {
		ROUND_start();
		while((status = ROUND_iterate(ARROW_NEUTRAL)) == FALLING);
	} while (status != DEAD);
}

int testRound() {
	int testsInError = 0;

	testsInError += bddPieceCanFall();
	testsInError += bddPieceCanNotFall();

	testsInError += bddPieceCanMove();
	testsInError += bddPieceCannotMoveLeft();
	testsInError += bddPieceCannotMoveRight();
	testsInError += bddPieceCannotRotateLeft();
	testsInError += bddPieceCannotRotateRight();

	testsInError += bddTestIterationWithPiece();
	
	testIteration();

	return testsInError;
}
#endif