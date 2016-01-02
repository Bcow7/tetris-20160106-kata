#ifndef ___KEYBOARD_H
#define ___KEYBOARD_H

typedef enum {
	ARROW_NEUTRAL,
	ARROW_ROTATE_LEFT,
	ARROW_ROTATE_RIGHT,
	ARROW_LEFT,
	ARROW_RIGHT
} Arrow;

Arrow KEYBOARD_keyPress();

#ifdef TEST
int testKeyboard();
#endif

#endif

