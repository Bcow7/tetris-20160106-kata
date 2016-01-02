#include <string.h>
#include "main.h"
#include "t6963c.h"
#include "bdd.h"
#include "board.h"

#define ROM_CG_ADDRESS 0x0000

/**
 * Dessine un rectangle entre les coordonn�es sp�cifi�es.
 * Le carr� est dessin� avec des caract�res BORDER_*, pour
 * que le serpent ne puisse pas le traverser.
 * @param x0, y0: Coordonn�es de l'angle sup�rieur droit.
 * @param x1, y1: Coordonn�es de l'angle inf�rieur gauche.
 */
void BOARD_draw(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
	// � faire...
}

/**
 * Remplit avec des espaces le rectangle d�fini par les coordonn�es.
 * Permet de nettoyer l'int�rieur du rectangle dessin� avec BOARD_draw.
 * @param x0, y0: Coordonn�es de l'angle sup�rieur droit.
 * @param x1, y1: Coordonn�es de l'angle inf�rieur gauche.
 */
void BOARD_clear(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
	// � faire...
}

/**
 * Copie la d�finition d'un caract�re depuis la ROM vers la zone de d�finition
 * des caract�res du T6963C.
 * @param positionInRom Position du caract�re dans la ROM.
 * @param cgCode Code du caract�re dans le t6963c.
 */
void BOARD_copyFromRomToCg(unsigned char positionInRom, unsigned char cgCode) {
	unsigned char *rom_cg_address = (unsigned char __xdata *) (ROM_CG_ADDRESS + positionInRom * 8);
	unsigned int video_cg_address = cgCode * 8 + T6963C_ZONE_CG;
	T6963C_autoWrite(video_cg_address, rom_cg_address, 8);
}

/**
 * Initialise les caract�res utilis�s pendant le jeu.
 */
void BOARD_initialize() {
	BOARD_copyFromRomToCg( 0, BORDER_A);
	BOARD_copyFromRomToCg( 1, BORDER_B);
	BOARD_copyFromRomToCg( 2, BORDER_C);
	BOARD_copyFromRomToCg( 3, BORDER_D);
	BOARD_copyFromRomToCg( 4, BORDER_E);
	BOARD_copyFromRomToCg( 5, BORDER_F);
	BOARD_copyFromRomToCg( 6, BORDER_G);
	BOARD_copyFromRomToCg( 7, BORDER_H);

	BOARD_copyFromRomToCg( 8, OBSTACLE);
	BOARD_copyFromRomToCg( 9, PIECE);

	BOARD_clear(TETRIS_LIMIT_X0 - 1, TETRIS_LIMIT_Y0 - 1, TETRIS_LIMIT_X1 + 1, TETRIS_LIMIT_Y1 + 1);
	BOARD_draw(TETRIS_LIMIT_X0 - 1, TETRIS_LIMIT_Y0 - 1, TETRIS_LIMIT_X1 + 1, TETRIS_LIMIT_Y1 + 1);
}

// Efface une ligne compl�te du tableau de jeu, et
// fait descendre de 1 cran tous les obstacles qui se
// trouvent au dessus.
void dropLine(unsigned char line) {
	// � faire...
}

/**
 * Efface toutes les lignes enti�res et fait descendre
 * les obstacles au dessus de 1 niveau.
 */
void BOARD_clearSolidRows() {
	// � faire...
}

/**
 * Affiche un texte entour� d'un rectangle.
 * La largeur du rectangle est d�finie par la longueur du texte.
 * La fonction ne v�rifie pas si le texte d�passe les bords de l'�cran.
 * @param x0, y0 L'angle sup�rieur droit du rectangle.
 * @param text Le texte � afficher.
 */
void BOARD_display(unsigned char x0, unsigned char y0, char *text) {
	// � faire...
}

#ifdef TEST
int bddBoardDraw() {
	BddContent c = {
		"ABBBBBBBBC",
		"D........E",
		"D........E",
		"D........E",
		"FGGGGGGGGH"
	};

	BDD_clear();
	BOARD_draw(BDD_SCREEN_X, BDD_SCREEN_Y, BDD_SCREEN_X + BDD_SCREEN_WIDTH - 1, BDD_SCREEN_Y + BDD_SCREEN_HEIGHT - 1);
	return BDD_assert(c, "BOD");
}

int bddBoardClear() {
	BddContent c = {
		"..........",
		".        .",
		".        .",
		".        .",
		".........."
	};

	BDD_clear();
	BOARD_clear(BDD_SCREEN_X + 1, BDD_SCREEN_Y + 1, BDD_SCREEN_X + BDD_SCREEN_WIDTH - 2, BDD_SCREEN_Y + BDD_SCREEN_HEIGHT - 2);
	return BDD_assert(c, "BOC");
}

int bddBoardDisplay() {
	BddContent c = {
		"..........",
		".ABBBBBC..",
		".D TXT E..",
		".FGGGGGH..",
		".........."
	};

	BDD_clear();
	BOARD_display(BDD_SCREEN_X + 1, BDD_SCREEN_Y + 1, " TXT ");
	return BDD_assert(c, "BOD");
}

int bddClearSolidRows1() {
	BddContent initial = {
		"%%%%%%%%%%",
		"%%%%%%%%% ",
		"%%%% %%%%%",
		" %%%%%%%%%",
		"%%%%%%%%%%"
	};
	BddContent expected = {
		"          ",
		"          ",
		"%%%%%%%%% ",
		"%%%% %%%%%",
		" %%%%%%%%%"
	};

	BDD_initialize(initial);
	BOARD_clearSolidRows();
	return BDD_assert(expected, "BOCLSR1");
}

int bddClearSolidRows2() {
	BddContent initial = {
		"%% % %  %%",
		" %% %%%%% ",
		"%%%%%%%%%%",
		" %    %%%%",
		"%%%%%%%%%%"
	};
	BddContent expected = {
		"          ",
		"          ",
		"%% % %  %%",
		" %% %%%%% ",
		" %    %%%%"
	};

	BDD_initialize(initial);
	BOARD_clearSolidRows();
	return BDD_assert(expected, "BOCLSR2");
}

int testBoard() {
	int testsInError = 0;

	testsInError += bddBoardDraw();
	testsInError += bddBoardClear();
	testsInError += bddBoardDisplay();
	testsInError += bddClearSolidRows1();
	testsInError += bddClearSolidRows2();
	return testsInError;
}
#endif
