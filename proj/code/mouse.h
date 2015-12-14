#ifndef __MOUSE_H
#define __MOUSE_H

typedef struct{
	unsigned int x, y;
	unsigned int left_btn ,middle_btn, right_btn;
	int deltax;
	int deltay;
} Mouse_t;

int mouse_subscribe_int();

int mouse_unsubscribe_int();

int mouse_output();

#endif /* __MOUSE_H */
