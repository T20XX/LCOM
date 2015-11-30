#ifndef __GAME_H
#define __GAME_H

#include "piece.h"
#include <stdio.h>

typedef enum {FALL, MOVE_RIGHT, MOVE_LEFT, FALL_QUICKER, ROTATE, SWAP_PIECES ,REACH_END} State;

typedef enum kbd_game_event {LEFTKEY_DOWN, RIGHTKEY_DOWN, UPKEY_DOWN, DOWNKEY_DOWN, SPACEBAR_DOWN, NOKEY} kbd_game_event;

//typedef enum {} mouse_game_event;

typedef struct {
	unsigned int game_mode;
	unsigned int time_elapsed;
	Piece * actual_piece;
	Piece * next_piece;
	State state, last_state;
	kbd_game_event kbd_event, last_kbd_event;
} Game;

Game * new_game(unsigned int mode);

void update_gamestate(Game * game);

void update_game(Game * game);

#endif /* __GAME_H */
