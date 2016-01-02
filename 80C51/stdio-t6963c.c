#include <mcs51reg.h>
#include "t6963c.h"

// Pointeur d'�criture pour putchar.
unsigned int address = 0;

/**
 * Initialise les d�pendances avant de pouvoir
 * utiliser printf et scanf.
 */
void STDIO_initialize() {
	T6963C_initialize();
}

/**
 * �tablit la valeur du pointeur d'�criture
 * d'apr�s les coordonn�es indiqu�es.
 * @param x Coordonn�e X en caract�res.
 * @param y Coordonn�e Y en caract�res.
 */
void STDIO_startWritingAt(unsigned char x, unsigned char y) {
	address = x + y * T6963C_COLONNES;
}

/**
 * Impl�mentation de putchar pour �crire avec
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

		case '�':
			T6963C_dataWrite(T6963C_ZONE_TEXTE + address++, 0x62);
			break;
			
		case '�':
			T6963C_dataWrite(T6963C_ZONE_TEXTE + address++, 0x65);
			break;

		default:
			T6963C_dataWrite(T6963C_ZONE_TEXTE + address++, c - 32);
			break;
	}
	return c;
}
