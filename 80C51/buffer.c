#include "test.h"

#define BUFFER_SIZE 250

/**
 * Le buffer est d�clar� comme m�moire externe.
 * L'adresse de la RAM est d�clar�e dans l'optios --xram-loc du linker.
 */
char __xdata buffer[BUFFER_SIZE];

/** Pointeur d'entr�e du buffer. */
unsigned char in = 0;

/** Pointeur de sortie du buffer. */
unsigned char out = 0;

/**
 * Ins�re un caract�re dans le buffer.
 * @param c Le caract�re.
 */
void BUFFER_in(char c) {
	buffer[in++] = c;
	in %= BUFFER_SIZE;
}

/**
 * R�cup�re un caract�re du buffer.
 * @return Le caract�re.
 */
char BUFFER_out() {
	char c = buffer[out++];
	out %= BUFFER_SIZE;
	return c;
}

/**
 * R�initialise le buffer.
 */
void BUFFER_clear() {
	in = 0;
	out = 0;
}

#ifdef TEST

int testBufferOneAndThreeChar() {
	int testsInError = 0;

	BUFFER_in(1);
	testsInError += assertEquals(1, BUFFER_out(), "BF_01");

	BUFFER_in(20);
	BUFFER_in(30);
	BUFFER_in(40);
	testsInError += assertEquals(20, BUFFER_out(), "BF_02");
	testsInError += assertEquals(30, BUFFER_out(), "BF_03");
	testsInError += assertEquals(40, BUFFER_out(), "BF_03");

	return testsInError;
}

int testBufferClear() {
	int testsInError = 0;

	BUFFER_in(14);
	BUFFER_in(15);
	BUFFER_clear();
	BUFFER_in(16);
	testsInError += assertEquals(16, BUFFER_out(), "BFC_01");

	return testsInError;
}

int testBufferLimit() {
	int n;

	BUFFER_clear();

	for (n = 0; n < BUFFER_SIZE; n++) {
		BUFFER_in(n % 20);
	}

	for (n = 0; n < BUFFER_SIZE; n++) {
		if (assertEquals(BUFFER_out(), n % 20, "BFL_01")) {
			return 1;
		}
	}

	return 0;
}

int testBufferLotsOfChar() {
	int n;

	for (n = BUFFER_SIZE / 2; n < BUFFER_SIZE * 2; n++) {
		BUFFER_in(n % 20);
		if (assertEquals(BUFFER_out(), n % 20, "BFL_02")) {
			return 1;
		}
	}

	return 0;
}

int testBuffer() {
	int testsInError = 0;

	testsInError += testBufferOneAndThreeChar();
	testsInError += testBufferClear();
	testsInError += testBufferLimit();
	testsInError += testBufferLotsOfChar();

	return testsInError;
}
#endif
