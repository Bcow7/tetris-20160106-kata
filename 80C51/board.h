#ifndef ___BOARD_H
#define ___BOARD_H

#ifdef TEST

#define CHAR_BASE 0x00
#define TETRIS_LIMIT_X0 BDD_SCREEN_X
#define TETRIS_LIMIT_Y0 BDD_SCREEN_Y
#define TETRIS_LIMIT_X1 BDD_SCREEN_X + BDD_SCREEN_WIDTH - 1
#define TETRIS_LIMIT_Y1 BDD_SCREEN_Y + BDD_SCREEN_HEIGHT - 1

int testBoard();

#else

#define CHAR_BASE 0x80
#define TETRIS_LIMIT_X0 7
#define TETRIS_LIMIT_Y0 1
#define TETRIS_LIMIT_X1 23
#define TETRIS_LIMIT_Y1 14

#endif

#define EMPTY 0x00
#define OBSTACLE (0x05 + CHAR_BASE)
#define PIECE (0x11 + CHAR_BASE)

#define BORDER_A (0x21 + CHAR_BASE)
#define BORDER_B (0x22 + CHAR_BASE)
#define BORDER_C (0x23 + CHAR_BASE)
#define BORDER_D (0x24 + CHAR_BASE)
#define BORDER_E (0x25 + CHAR_BASE)
#define BORDER_F (0x26 + CHAR_BASE)
#define BORDER_G (0x27 + CHAR_BASE)
#define BORDER_H (0x28 + CHAR_BASE)


void BOARD_initialize();
void BOARD_draw(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void BOARD_clear(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void BOARD_display(unsigned char x0, unsigned char y0, char *text);
void BOARD_clearSolidRows();

#endif
