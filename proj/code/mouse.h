/**
 * This module contains all functions to manage mouse
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight:
 */
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

/**
 * @brief Subscribes the mouse interrupts
 *
 * @return Return mouse ID to verify the interrupts and -1 upon failure
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes the mouse interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int mouse_unsubscribe_int();

/**
 * @brief Reads from the output buffer of the mouse
 *
 * @return Return data of buffer and -1 upon failure
 */
int mouse_output();

#endif /* __MOUSE_H */
