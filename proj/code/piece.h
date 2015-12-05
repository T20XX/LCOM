#ifndef __PIECE_H
#define __PIECE_H

#include "sprite.h"
#include "piecespix.h"

#define FACE_LENGTH 		30;

typedef struct{
	Sprite sprite;
} Piece;

Piece *new_piece(unsigned int xi, unsigned int yi,int yspeed);

void draw_piece(Piece * piece);

#endif /* __PIECE_H */
