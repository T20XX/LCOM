#include <stdint.h>

#include "logic.h"


//change everything to FACE_LENGTH
int can_piece_move_x(Piece * piece, Board * board, unsigned int dir){
	uint16_t * piece_ptr;
	uint16_t * board_ptr;

	int relative_x;
	unsigned int i;
	for(i=0; i<(piece->sprite.height / 30);i++){				//??
		piece_ptr = piece->sprite.map + piece->sprite.width * i * 30; //??
		if (dir == 1){
			piece_ptr += piece->sprite.width -1;	//?? To get the last "pixel"of the line
		}

		relative_x = 0;								//relative x of block in piece
		while((*piece_ptr) == 0){
			if (dir == 0){
				relative_x += 30;								//??
			}else if (dir == 1){
				relative_x -= 30;								//??
			}
			piece_ptr += relative_x;
		}

		board_ptr = board->map;

		if (dir == 0){
			board_ptr +=
					((piece->sprite.x - board->x) + relative_x - 1) +
					(board->width * ((piece->sprite.y - board->y) + i * 30));
		}else if (dir == 1){
			board_ptr +=
					((piece->sprite.x - board->x) + piece->sprite.width + relative_x) +
					(board->width * ((piece->sprite.y - board->y) + i * 30));
		}
		if ((*board_ptr) != 0){			//Verifies the position is not black
			return 1;
		}
	}

	return 0;
}

int can_piece_fall(Piece * piece, Board * board){
	uint16_t * piece_ptr;
	uint16_t * board_ptr;

	int relative_y;
	unsigned int i;
	for(i=0; i<(piece->sprite.width / 30);i++){				//??
		piece_ptr = piece->sprite.map +
				(piece->sprite.width * (piece->sprite.height -1)) +
				i * 30; //??

		relative_y = 0;
		while((*piece_ptr) == 0){							//"Climbs" piece sprite to find "pixel" not black
			relative_y -= 30;
			piece_ptr += piece->sprite.width * 30;			//??
		}

		board_ptr = board->map;

		board_ptr +=
				((piece->sprite.x - board->x) + i*30) +
				(board->width * ((piece->sprite.y - board->y) + piece->sprite.height + relative_y));

		if ((*board_ptr) != 0){			//Verifies the position is not black
			return 1;
		}
	}

	return 0;
}
