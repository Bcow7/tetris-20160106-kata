#include "keyboard.h"
#include "test.h"

#define KEY_NONE 0xFF
#define KEY_8 0xFD
#define KEY_4 0xFE
#define KEY_6 0xFB
#define KEY_2 0xFD

/**
 * Detecte les touches 8, 4, 6 et 2 comme des fleches.
 * - '8' --> Fleche montante.
 * - '4' --> Fleche vers la gauche.
 * - '6' --> Fleche vers la droite.
 * - '2' --> Fleche vers le bas.
 * @param keyboard Tableau avec l'état des 4 lignes du clavier.
 * @return La fleche correspondante à la touche enfoncée, ou ARROW_NEUTRAL
 * si aucune touche est enfoncée.
 */
Arrow KEYBOARD_readArrows(unsigned char *keyboard) {
	if (keyboard[0] == KEY_8) {
		return ARROW_ROTATE_LEFT;
	}
	if (keyboard[1] == KEY_4) {
		return ARROW_LEFT;
	}
	if (keyboard[1] == KEY_6) {
		return ARROW_RIGHT;
	}
	if (keyboard[2] == KEY_2) {
		return ARROW_ROTATE_RIGHT;
	}
	return ARROW_NEUTRAL;
}

Arrow KEYBOARD_readKeyPress(unsigned char *keyboard) {
	static Arrow lastArrow = ARROW_NEUTRAL;
	Arrow arrow = KEYBOARD_readArrows(keyboard);

	if (arrow != lastArrow) {
		lastArrow = arrow;
		return arrow;
	}

	return ARROW_NEUTRAL;
}

Arrow KEYBOARD_keyPress() {
	static 	unsigned char *keyboard = (unsigned char __xdata *) 0x3000;

	return KEYBOARD_readKeyPress(keyboard);
}

#ifdef TEST

int testKeyboardArrows() {
	int testsInError = 0;
	unsigned char keys[4] = {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE};

	testsInError += assertEquals(KEYBOARD_readArrows(keys), ARROW_NEUTRAL, "KB001");
	
	keys[0] = KEY_8;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_ROTATE_LEFT, "KB002");
	keys[0] = KEY_NONE;
	keys[1] = KEY_4;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_LEFT, "KB003");
	keys[1] = KEY_6;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_RIGHT, "KB004");
	keys[1] = KEY_NONE;
	keys[2] = KEY_2;
	testsInError +=assertEquals(KEYBOARD_readArrows(keys), ARROW_ROTATE_RIGHT, "KB005");

	return testsInError;
}

int testKeyboardKeyPress() {
	int testsInError = 0;
	unsigned char keys[4] = {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE};
	unsigned char keys4[4] = {KEY_NONE, KEY_4, KEY_NONE, KEY_NONE};
	unsigned char keys2[4] = {KEY_NONE, KEY_NONE, KEY_2, KEY_NONE};

	testsInError += assertEquals(KEYBOARD_readKeyPress(keys), ARROW_NEUTRAL, "KBP1");
	testsInError += assertEquals(KEYBOARD_readKeyPress(keys4), ARROW_LEFT, "KBP2");
	testsInError += assertEquals(KEYBOARD_readKeyPress(keys4), ARROW_NEUTRAL, "KBP3");
	testsInError += assertEquals(KEYBOARD_readKeyPress(keys2), ARROW_ROTATE_RIGHT, "KBP4");
	testsInError += assertEquals(KEYBOARD_readKeyPress(keys2), ARROW_NEUTRAL, "KBP5");
	testsInError += assertEquals(KEYBOARD_readKeyPress(keys), ARROW_NEUTRAL, "KBP6");
	testsInError += assertEquals(KEYBOARD_readKeyPress(keys4), ARROW_LEFT, "KBP7");

	return testsInError;
}


int testKeyboard() {
	int testsInError = 0;

	testsInError += testKeyboardArrows();
	testsInError += testKeyboardKeyPress();
	
	return testsInError;
}

#endif
