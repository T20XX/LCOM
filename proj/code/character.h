#ifndef __CHARACTER_H
#define __CHARACTER_H

#include <stdio.h>

typedef enum {
	CHAR_JUMP,
	CHAR_FALL,
	CHAR_MOVE_RIGHT,
	CHAR_MOVE_LEFT,
	STOPPED
} character_state;

typedef enum kbd_character_event {
	A_DOWN, A_UP,
	W_DOWN, W_UP,
	S_DOWN, S_UP,
	D_DOWN, D_UP,
	NO_KEY
} kbd_character_event;

typedef enum timer_character_event {MOVE_FRAME_TICK, NOTICK} timer_character_event;

#define CHAR_INIT_X						500.0
#define CHAR_INIT_Y						100.0

#define CHARACTER_LENGTH				30

#define GRAVITY_SPEED					0.16
#define CHAR_X_SPEED					2
#define CHAR_JUMP_SPEED					7

#define CHAR_MOVE_FRAME_DELAY			10
#define NUMBER_OF_SPRITES				4

typedef struct {
	float x,y;
	float xspeed,yspeed;
	unsigned int width, height;
	uint16_t * current;
	uint16_t * left;
	uint16_t * right;
	character_state state;
	character_state last_state;
	kbd_character_event kbd_event;
	timer_character_event timer_event;
	unsigned int falling, jumping;
	unsigned int sprite_counting;
} Character;

Character * new_character();

void update_character_state(Character * character);

void update_character(Character * character);

void draw_character(Character * character);

void delete_character(Character * character);

#endif /* __GAME_H */
