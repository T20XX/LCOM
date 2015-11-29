#ifndef __GAME_H
#define __GAME_H

#include "piece.h"

typedef struct {
	unsigned int game_mode;
	Piece * actual_piece;
	Piece * next_piece;
} Game;

Game * new_game(unsigned int mode);

#endif /* __GAME_H */
