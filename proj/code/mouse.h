#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>

typedef struct{
	unsigned int x, y;
	unsigned int left_btn ,middle_btn, right_btn;
	int deltax;
	int deltay;
	unsigned int width,height;
	uint16_t * map;
} Mouse_t;

int mouse_subscribe_int();

int mouse_unsubscribe_int();

int mouse_output();

#endif /* __MOUSE_H */
