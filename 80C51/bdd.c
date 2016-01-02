#include <stdio.h>
#include "t6963c.h"
#include "bdd.h"

#ifdef TEST

/**
 * Nettoye et réaffiche la zone d'écran de test.
 */
void BDD_clear() {
	unsigned char n;
	unsigned int address;

	address = T6963C_calculateAddress(BDD_SCREEN_X - 1, BDD_SCREEN_Y - 1);
	T6963C_autoRepeat(address, 0x03, BDD_SCREEN_WIDTH + 2);

	for (n=0; n < BDD_SCREEN_HEIGHT; n++) {
		address = T6963C_calculateAddress(BDD_SCREEN_X - 1, BDD_SCREEN_Y + n);
		T6963C_dataWrite(address, 0x03);
		address += 1;

		T6963C_autoRepeat(address, 0x0E, BDD_SCREEN_WIDTH);

		address += BDD_SCREEN_WIDTH;
		T6963C_dataWrite(address, 0x03);
	}
	
	address = T6963C_calculateAddress(BDD_SCREEN_X - 1, BDD_SCREEN_Y + BDD_SCREEN_HEIGHT);
	T6963C_autoRepeat(address, 0x03, BDD_SCREEN_WIDTH + 2);
}

/**
 * Copie le contenu initial dans l'écran de test.
 * @param initialContent le contenu à copier dans l'écran de test.
 */
void BDD_initialize(BddContent initialContent) {
	unsigned char n, m;
	unsigned int address;

	BDD_clear();
	for (n=0; n < BDD_SCREEN_HEIGHT; n++) {
		address = T6963C_calculateAddress(BDD_SCREEN_X, BDD_SCREEN_Y + n);
		for (m = 0; m < BDD_SCREEN_WIDTH; m++) {
			T6963C_dataWrite(address++, initialContent[n][m] - 32);
		}
	}	
}

/**
 * Vérifie le contenu de l'écran de test.
 * @param Le contenu attendu.
 * @param testId Identifiant du test affiché si il échoue.
 * @return 1 si le contenu de l'écran de test est différent de celui 
 * attendu.
 */
int BDD_assert(BddContent expectedContent, char *testId) {
	unsigned char x, y;
	unsigned char foundContent, e;
	int unexpectedContent = 0;

	for (y = 0; y < BDD_SCREEN_HEIGHT; y++) {
		for (x = 0; x < BDD_SCREEN_WIDTH; x++) {
			e = expectedContent[y][x] - 32;
			foundContent = T6963C_readFrom(BDD_SCREEN_X + x, BDD_SCREEN_Y + y);
			if (foundContent != e) {
				T6963C_writeAt(BDD_SCREEN_X + x, BDD_SCREEN_Y + y, 'X' - 0x20);
				unexpectedContent = 1;
			}
		}
	}

	if (unexpectedContent) {
		printf("%s: Erreur bdd\r\n", testId);
	}

	return unexpectedContent;
}

#endif