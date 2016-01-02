#include "test.h"
#include "bdd.h"
#include "t6963c.h"
#include "buffer.h"
#include "gameboard.h"
#include "snake.h"

#define FRUIT_CALORIES 3

/**
 * Modifie les coordonnées du serpent selon sa direction.
 * @param snake La description du serpent.
 */
void SNAKE_move(Snake *snake) {
	switch(snake->direction) {
		case MOVES_UP:
			snake->position.y--;
			break;
		case MOVES_DOWN:
			snake->position.y++;
			break;
		case MOVES_LEFT:
			snake->position.x--;
			break;
		case MOVES_RIGHT:
			snake->position.x++;
			break;
	}
}

/**
 * Décide si le serpent vit ou meurt, ou mange un fruit, selon
 * sa position et ce qui se trouve à cet endroit.
 * @param snake La description du serpent.
 */
void SNAKE_liveOrDie(Snake *snake) {
	unsigned char c = T6963C_readFrom(snake->position.x, snake->position.y);

	switch (c) {
		case OBSTACLE_A:
		case OBSTACLE_B:
		case OBSTACLE_C:
		case OBSTACLE_D:
		case OBSTACLE_E:
		case OBSTACLE_F:
		case OBSTACLE_G:
		case OBSTACLE_H:
		case SNAKE_BODY:
			snake->status = DEAD;
			break;

		case FRUIT:
			snake->caloriesLeft += FRUIT_CALORIES;
			snake->status = EATING;
			break;

		default:
			snake->status = ALIVE;
	}

	if (snake->position.y == SNAKE_LIMIT_Y0) {
		snake->status = DEAD;
	}
	if (snake->position.y == SNAKE_LIMIT_Y1) {
		snake->status = DEAD;
	}
	if (snake->position.x == SNAKE_LIMIT_X0) {
		snake->status = DEAD;
	}
	if (snake->position.x == SNAKE_LIMIT_X1) {
		snake->status = DEAD;
	}
}

/**
 * Affiche la tête du serpent.
 * @param snake La définition du serpent.
 */
void SNAKE_showHead(Snake *snake) {
	unsigned char head;

	switch(snake->status) {
		case DEAD:
			head = SNAKE_DEAD;
			break;
		default:
			head = SNAKE_HEAD;
			break;
	}

	T6963C_writeAt(snake->position.x, snake->position.y, head);
}

/**
 * Affiche le corps du serpent, et efface la queue du serpent.
 * Aussi, si le serpent a mangé un fruit, affiche un caractère spécial.
 * @param snake La définition du serpent.
 */
void SNAKE_showBody(Snake *snake) {
	unsigned char tailX, tailY;
	unsigned char body = SNAKE_BODY;

	BUFFER_in(snake->position.x);
	BUFFER_in(snake->position.y);

	if (snake->status == EATING) {
		body = SNAKE_SWALLOW;
	}
	T6963C_writeAt(snake->position.x, snake->position.y, body);

	if (snake->caloriesLeft > 0) {
		snake->caloriesLeft--;
	} else {
		tailX = BUFFER_out();
		tailY = BUFFER_out();
		T6963C_writeAt(tailX, tailY, EMPTY);
	}
}

/**
 * Décide de varier la direction du serpent selon la direction indiquée.
 * Le serpent ne peut jamais tourner de 180º en un mouvement.
 * @param snake La description du serpent.
 * @param arrow La direction désirée.
 */
void SNAKE_turn(Snake *snake, Arrow arrow) {
	switch (snake->direction) {
		case MOVES_UP:
		case MOVES_DOWN:
			switch(arrow) {
				case ARROW_LEFT:
					snake->direction = MOVES_LEFT;
					break;
				case ARROW_RIGHT:
					snake->direction = MOVES_RIGHT;
					break;
			}
			break;

		case MOVES_RIGHT:
		case MOVES_LEFT:
			switch(arrow) {
				case ARROW_UP:
					snake->direction = MOVES_UP;
					break;
				case ARROW_DOWN:
					snake->direction = MOVES_DOWN;
					break;
			}
			break;
	}
}

/**
 * Effectue une itération dans la vie du serpent.
 * @param snake La définition du serpent.
 * @return L'état du serpent après l'itération.
 */
Status SNAKE_iterate(Snake *snake, Arrow arrow) {
	SNAKE_showBody(snake);
	SNAKE_turn(snake, arrow);
	SNAKE_move(snake);
	SNAKE_liveOrDie(snake);
	SNAKE_showHead(snake);

	return snake->status;
}

#ifdef TEST

// ========================== Tests unitaires =================================
// Chaque test vérifie le comportement d'une fonctionnalité en établissant
// un état initial et en vérifiant l'état final.
int testSnakeTurnsTo(Direction currentDirection, Arrow turn, Direction expectedResult, char *testCode) {
	Snake snake = {MOVES_RIGHT, {10, 10}, ALIVE, 0};
	snake.direction = currentDirection;
	SNAKE_turn(&snake, turn);
	return assertEquals(expectedResult, snake.direction, testCode);	
}

int testSnakeTurns() {
	int testsInError = 0;

	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_RIGHT,	MOVES_RIGHT,"ST01");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_LEFT,	MOVES_RIGHT,"ST02");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_UP,		MOVES_UP, 	"ST03");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_DOWN, 	MOVES_DOWN, "ST04");

	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_RIGHT, 	MOVES_LEFT, "ST11");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_LEFT, 	MOVES_LEFT, "ST12");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_UP, 		MOVES_UP, 	"ST13");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_DOWN, 	MOVES_DOWN, "ST14");

	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_RIGHT, 	MOVES_RIGHT,"ST21");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_LEFT, 		MOVES_LEFT, "ST22");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_UP, 		MOVES_UP, 	"ST23");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_DOWN, 		MOVES_UP, 	"ST24");

	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_RIGHT, 	MOVES_RIGHT,"ST31");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_LEFT, 	MOVES_LEFT, "ST32");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_UP, 		MOVES_DOWN, "ST33");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_DOWN, 	MOVES_DOWN, "ST34");

	return testsInError;
}

int testSnakeEatsAFruit() {
	int testsInError = 0;
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 0};
	
	T6963C_writeAt(BDD_SCREEN_X, BDD_SCREEN_Y, FRUIT);
	SNAKE_liveOrDie(&snake);

	testsInError += assertEquals(snake.status, EATING, "SF001");
	testsInError += assertEquals(snake.caloriesLeft, FRUIT_CALORIES, "SF002");

	T6963C_writeAt(BDD_SCREEN_X, BDD_SCREEN_Y, EMPTY);
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, ALIVE, "SF003");

	return testsInError;	
}

int testSnakeMoves() {
	int testsInError = 0;
	Snake snake;

	snake.position.x = 10;
	snake.position.y = 10;

	snake.direction = MOVES_UP;
	SNAKE_move(&snake);
	testsInError += assertEquals(9, snake.position.y, "SM001");

	snake.direction = MOVES_DOWN;
	SNAKE_move(&snake);
	testsInError += assertEquals(10, snake.position.y, "SM002");

	snake.direction = MOVES_LEFT;
	SNAKE_move(&snake);
	testsInError += assertEquals( 9, snake.position.x, "SM003");

	snake.direction = MOVES_RIGHT;
	SNAKE_move(&snake);
	testsInError += assertEquals(10, snake.position.x, "SM004");

	return testsInError;
}

int testSnakeHitsABorder() {
	int testsInError = 0;

	Snake snake;

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0 + 1;
	snake.position.y = SNAKE_LIMIT_Y0 + 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, ALIVE, "SO01");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0;
	snake.position.y = SNAKE_LIMIT_Y0 + 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO02");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0 + 1;
	snake.position.y = SNAKE_LIMIT_Y0;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO03");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X1;
	snake.position.y = SNAKE_LIMIT_Y1 - 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO04");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X1 - 1;
	snake.position.y = SNAKE_LIMIT_Y1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO05");

	return testsInError;
}

// =========================== Tests de comportement ============================
// Chaque test:
// 1- Établit un état initial.
// 2- Simule un scénario qu'un utilisateur pourrait réaliser manuellement.
// 3- Vérifie, en contrôlant le contenu de l'écran, que ce 
//    que percevrait l'utilisateur est juste

int bddSnakeHitsItsTail() {
	BddExpectedContent c = {
		" 14111....",
		"..1..1....",
		"..1111....",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 11};
	char n;

	BUFFER_clear();
	BDD_clear();
	
	for (n = 0; n < 5; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	for (n = 0; n < 2; n++) {
		SNAKE_iterate(&snake, ARROW_DOWN);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_LEFT);
	}
	for (n = 0; n < 2; n++) {
		SNAKE_iterate(&snake, ARROW_UP);
	}

	return BDD_assert(c, "SNT");
}


int bddSnakeHitsThisObstacle(char obstacle, char *testId) {
	BddExpectedContent c = {
		"  1114....",
		"..........",
		"..........",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	T6963C_writeAt(BDD_SCREEN_X + 5, BDD_SCREEN_Y, obstacle);
	
	for (n = 0; n < 5; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}

	return BDD_assert(c, testId);
}

int bddSnakeHitsAnObstacle() {
	int testsInError = 0;

	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_A, "SNO-A");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_B, "SNO-B");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_C, "SNO-C");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_D, "SNO-D");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_E, "SNO-E");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_F, "SNO-F");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_G, "SNO-G");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_H, "SNO-H");

	return testsInError;
}

int bddSnakeMovesTurnsAndCatchesAFruit() {
	BddExpectedContent c = {
		"      1...",
		"......2...",
		"......1...",
		"...3111...",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	T6963C_writeAt(BDD_SCREEN_X + 6, BDD_SCREEN_Y + 1, FRUIT);
	
	for (n = 0; n < 6; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_DOWN);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_LEFT);
	}

	return BDD_assert(c, "SNTF");
}

int bddSnakeGrows() {
	BddExpectedContent c = {
		"    1113..",
		"..........",
		"..........",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	for (n=0; n<7; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	
	return BDD_assert(c, "SNG");
}

/**
 * Collection de tests.
 * Les tests en erreur sont affichés à l'écran.
 * @return Le nombre de tests échoués.
 */
int testSnake() {
	int testsInError = 0;

	// Tests unitaires:
	testsInError += testSnakeTurns();
	testsInError += testSnakeMoves();
	testsInError += testSnakeHitsABorder();
	testsInError += testSnakeEatsAFruit();

	// Tests de comportement:
	testsInError += bddSnakeGrows();
	testsInError += bddSnakeMovesTurnsAndCatchesAFruit();
	testsInError += bddSnakeHitsAnObstacle();
	testsInError += bddSnakeHitsItsTail();

	// Nombre de tests en erreur:
	return testsInError;
}

#endif
