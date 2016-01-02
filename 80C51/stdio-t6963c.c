#include <mcs51reg.h>
#include "t6963c.h"

// Pointeur d'écriture pour putchar.
unsigned int address = 0;

/**
 * Initialise les dépendances avant de pouvoir
 * utiliser printf et scanf.
 */
void STDIO_initialize() {
	T6963C_initialize();
}

/**
 * Établit la valeur du pointeur d'écriture
 * d'après les coordonnées indiquées.
 * @param x Coordonnée X en caractères.
 * @param y Coordonnée Y en caractères.
 */
void STDIO_startWritingAt(unsigned char x, unsigned char y) {
	address = x + y * T6963C_COLONNES;
}

/**
 * Implémentation de putchar pour écrire avec
 * la librairie T6963C.
 */
char putchar(char c) {
	switch(c) {
		case '\n':
			address += T6963C_COLONNES;
			break;

		case '\r':
			address /= T6963C_COLONNES;
			address *= T6963C_COLONNES;
			break;

		case '\t':
			address += 3;
			break;

		case 'é':
			T6963C_dataWrite(T6963C_ZONE_TEXTE + address++, 0x62);
			break;
			
		case 'à':
			T6963C_dataWrite(T6963C_ZONE_TEXTE + address++, 0x65);
			break;

		default:
			T6963C_dataWrite(T6963C_ZONE_TEXTE + address++, c - 32);
			break;
	}
	return c;
}
