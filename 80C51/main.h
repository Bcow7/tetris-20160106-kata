#ifndef __MAIN_H
#define __MAIN_H

typedef struct {
	unsigned char x;
	unsigned char y;
} Position;

typedef struct {
	unsigned char width;
	unsigned char height;
	unsigned char *data;
} Block;

#endif
