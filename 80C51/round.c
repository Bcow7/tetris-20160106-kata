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
 * Indique si la pi�ce, dans sa rotation et position actuelle, peut
 * encore tomber de un cran.
 * @param piece La description de la pi�ce.
 * @return 1 si la pi�ce peut encore descendre. 0 autrement.
 */
unsigned char ROUND_pieceCanFall(Piece *piece) {
	// � faire
	return 0;	
}

/**
 * Indique si la pi�ce peut se d�placer dans la direction
 * de la fl�che.
 * @param piece La description de la pi�ce.
 * @param arrow La direction de mouvement.
 * @return 1 si la pi�ce peut se d�placer dans cette direction.
 */
unsigned char ROUND_pieceCanMove(Piece *piece, Arrow arrow) {
	// � faire
	return 0;
}

/**
 * Fait tomber la pi�ce de un niveau, � moins qu'elle ne soit bloqu�e
 * par le fond du tableau de jeu ou un obstacle.
 * Si la pi�ce est bloqu�e et elle ne peut pas tomber, alors elle est fig�e.
 * @param piece La description de la pi�ce.
 * @return L'�tat de la pi�ce:
 * - FALLING si la pi�ce n'est pas encore fig�e.
 * - FROZEN si la pi�ce ne peut plus tomber.
 */
PieceStatus ROUND_fallOrFreeze(Piece *piece) {
	// � faire
	return FROZEN;
}

/**
 * D�pace (si possible) la pi�ce selon la fl�che indiqu�e.
 * La pi�ce se d�place � gauche ou � droite, ou bien tourne sur elle m�me.
 * Si la pi�ce est bloqu�e par un bord ou un obstacle, alors elle reste dans
 * sa position actuelle.
 * @param piece La description de la pi�ce.
 * @param arrow La direction de mouvement.
 */
void ROUND_move(Piece *piece, Arrow arrow) {
	// � faire
}

// Nombre d'it�rations avec la pi�ce avant qu'elle ne tombe de un niveau.
unsigned char verticalPause;

/**
 * �tablit le nombre d'it�ration avec la pi�ce avant qu'elle ne tombe de un niveau.
 * @param pause Plus cette valeur est grande, plus la pi�ce tombe lentement.
 */
void ROUND_setVerticalPause(unsigned char pause) {
	verticalPause = pause;
}

// Compte le nombre d'it�rations avec la pi�ce avant qu'elle ne tombe de un niveau.
unsigned char time = 0;

// Appel�e � chaque it�ration avec la pi�ce,
// cette fonction indique quand il est temps de faire descendre la pi�ce
// de 1 niveau.
char timeToFall() {
	if (time-- == 0) {
		time = verticalPause;
		return 1;
	}
	return 0;
}

/**
 * Effectue une it�ration avec la pi�ce.
 * - La pi�ce peut se d�placer � tout moment, selon la fl�che indiqu�e.
 * - R�guli�rement, la pi�ce tombe de un niveau.
 * - Si la pi�ce ne peut plus tomb�e, alors elle reste fig�e.
 * - Si la pi�ce reste fig�e au sommet du tableau de jeu, alors la partie est finie.
 * @param piece La description de la pi�ce.
 * @param arrow La direction de mouvement.
 * @return L'�tat de la pi�ce:
 * - FALLING si la pi�ce n'est pas encore fig�e.
 * - FROZEN si la pi�ce ne peut plus tomber.
 * - DEAD si la pi�ce est fig�e au sommet du tableau de jeu.
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
 * Initialise une pi�ce au sommet du tableau de jeu.
 * @param piece La pi�ce � initialiser.
 */
void ROUND_startPiece(Piece *piece) {
	TetrominoType type = rand() % 7;
	time = verticalPause;
	PIECE_initialize(piece, type, (TETRIS_LIMIT_X1 + TETRIS_LIMIT_X0) / 2, TETRIS_LIMIT_Y0, 0);
}

// Piece active pendant ce round
Piece piece;

/**
 * Initialise une nouvelle pi�ce.
 */
void ROUND_start() {
	ROUND_startPiece(&piece);
}

/**
 * Ex�cute une it�ration.
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