#ifndef __GAME_H
#define __GAME_H

#include "piece.h"
#include <stdio.h>

typedef enum {FALL, MOVE_RIGHT, MOVE_LEFT, ROTATE, SWAP_PIECES ,REACH_END, DO_NOTHING, GAME_OVER} State;

typedef enum kbd_game_event {LEFTKEY_DOWN, RIGHTKEY_DOWN, UPKEY_DOWN, DOWNKEY_DOWN, SPACEBAR_DOWN, NOKEY} kbd_game_event;

typedef enum timer_game_event {FALL_TICK, NO_TICK} timer_game_event;

typedef enum mouse_game_event {MOUSE_RIGHT, MOUSE_LEFT, MOUSE_STOPPED} mouse_game_event;

//Game constants
#define ONE_PLAYER_BOARD_X					332
#define ONE_PLAYER_BOARD_Y					70
#define ONE_PLAYER_RELATIVE_NEXT_PIECE_X	420
#define ONE_PLAYER_RELATIVE_NEXT_PIECE_Y	90
#define RELATIVE_COUNTER_Y					0
#define RELATIVE_NEXT_STRING_Y				50
#define RELATIVE_POINTS_STRING_Y			200
#define RELATIVE_POINTS_Y					260
#define RELATIVE_LINES_STRING_Y				400
#define RELATIVE_LINES_Y					460


#define TWO_PLAYER_BOARD_X					150
#define TWO_PLAYER_BOARD_Y					70
#define TWO_PLAYER_RELATIVE_NEXT_PIECE_X	420
#define TWO_PLAYER_RELATIVE_NEXT_PIECE_Y	90

#define BOARD_RELATIVE_MIDDLE_X				150

#define INIT_FALL_DELAY						40

typedef struct {
  unsigned int x,y;
  unsigned int width, height;
  uint16_t * map;
} Board;

typedef struct {
	unsigned int game_mode;
	Piece * actual_piece;
	Piece * next_piece;
	State state, last_state;
	kbd_game_event kbd_event, last_kbd_event;
	mouse_game_event mouse_event;
	unsigned int fall_delay;
	timer_game_event timer_event;
	Board board;
	unsigned int pieces_already_swapped;
	unsigned int points;
	unsigned int lines;
} Game;

Game * new_game(unsigned int mode);

void update_gamestate(Game * game);

void update_game(Game * game);

void draw_game(Game * game);

void delete_game(Game * game);

void add_piece(Piece * piece, Board * board);

int remove_finished_lines(Board * board);

void rotate_piece(Piece * piece, Board * board);

void swap_pieces(Piece * actual, Piece * next, Board * board);

#endif /* __GAME_H */
