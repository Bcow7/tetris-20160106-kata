#include <stdlib.h>
#include "main.h"
#include "fruit.h"
#include "t6963c.h"
#include "gameboard.h"
#include "test.h"

/**
 * Place un fruit � la position indiqu�e.
 * Si la position indiqu�e n'est pas libre, la fonction
 * cherche un endroit voisin qui soit libre.
 * @param position Position pour placer le fruit. Si n�cessaire
 * le param�tre est mis � jour avec la position effective du fruit.
 */
void FRUIT_placeInFreeSpace(Position *position) {
	char c;
	while ( (c = T6963C_readFrom(position->x, position->y)) != EMPTY) {
		position->x++;
	}
	T6963C_writeAt(position->x, position->y, FRUIT);
}

/**
 * Calcule une position al�atoire pour placer un fruit.
 * La fonction ne v�rifie pas si la position est libre. Par contre
 * elle ne d�passe pas le rectangle d�fini par les constantes 
 * SNAKE_LIMIT_X0, SNAKE_LIMIT_Y0 et SNAKE_LIMIT_X1, SNAKE_LIMIT_Y1.
 * @param position Param�tre de sortie, avec la position du fruit.
 */
void FRUIT_random(Position *position) {
	position->x = rand() % (SNAKE_LIMIT_X1 - SNAKE_LIMIT_X0) + SNAKE_LIMIT_X0;
	position->y = rand() % (SNAKE_LIMIT_Y1 - SNAKE_LIMIT_Y0) + SNAKE_LIMIT_Y0;
}

/**
 * Affiche un fruit � une position al�atoire, libre, et � l'int�rieur
 * du rectangle de jeu d�fini par 
 * SNAKE_LIMIT_X0, SNAKE_LIMIT_Y0 et SNAKE_LIMIT_X1, SNAKE_LIMIT_Y1.
 */
void FRUIT_place() {
	Position position;
	FRUIT_random(&position);
	FRUIT_placeInFreeSpace(&position);
}

#ifdef TEST
int testFruitIsPlacedInFreeSpace() {
	int testsInError = 0;
	Position position = {10, 10};
	char c;

	T6963C_writeAt(position.x, position.y, EMPTY);

	FRUIT_placeInFreeSpace(&position);
	testsInError += assertEquals(position.x, 10, "FPF1");
	testsInError += assertEquals(position.y, 10, "FPF2");
	c = T6963C_readFrom(10, 10);
	testsInError += assertEquals(c, FRUIT, "FPF3");

	FRUIT_placeInFreeSpace(&position);
	testsInError += assertNotEquals(position.x, 10, "FPF4");
	c = T6963C_readFrom(position.x, position.y);
	testsInError += assertEquals(c, FRUIT, "FPF6");

	return testsInError;
}

int testFruitIsPlacedRandomly() {
	int testsInError = 0;

	Position position1 = {50, 50};
	Position position2 = {50, 50};
	
	FRUIT_random(&position1);
	FRUIT_random(&position2);

	testsInError += assertNotEquals(position1.x, 50, "FPR1");
	testsInError += assertNotEquals(position1.y, 50, "FPR2");
	testsInError += assertNotEquals(position2.x, 50, "FPR3");
	testsInError += assertNotEquals(position2.y, 50, "FPR4");
	
	testsInError += assertNotEquals(position1.x, position2.x, "FPR5");
	testsInError += assertNotEquals(position1.y, position2.y, "FPR6");


	return testsInError;
}

int testFruit() {
	int testsInError = 0;
	
	testsInError += testFruitIsPlacedRandomly();
	testsInError += testFruitIsPlacedInFreeSpace();

	return testsInError;
}
#endif
