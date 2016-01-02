#ifndef ___SNAKE_H
#define ___SNAKE_H

#include "keyboard.h"
#include "main.h"

typedef enum {
	MOVES_UP,
	MOVES_DOWN,
	MOVES_LEFT,
	MOVES_RIGHT
} Direction;

typedef enum {
	ALIVE,
	DEAD,
	EATING
} Status;

typedef struct {
	Direction direction;
	Position position;
	Status status;
	unsigned char caloriesLeft;
} Snake;

void SNAKE_move(Snake *snake);
void SNAKE_liveOrDie(Snake *snake);
void SNAKE_show(Snake *snake);
void SNAKE_turn(Snake *snake, Arrow arrow);
Status SNAKE_iterate(Snake *snake, Arrow arrow);

#ifdef TEST
int testSnake();
#endif

#endif
