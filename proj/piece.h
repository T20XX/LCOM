#ifndef __PIECE_H
#define __PIECE_H

#include "sprite.h"
#include "piecespix.h"

typedef struct{
	Sprite sprite;
} Piece;

Piece *new_piece(unsigned int xi, unsigned int yi,int yspeed);
#endif /* __PIECE_H */
