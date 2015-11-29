#include "piece.h"

Piece *new_piece(unsigned int xi, unsigned int yi, int yspeed){
	int random = rand() % 6;
	Piece * piece = (Piece*) malloc(sizeof(Piece));
	int tempwidth;
	int tempheight;
	piece->sprite.x = xi;
	piece->sprite.y = yi;
	piece->sprite.xspeed = 0;
	piece->sprite.yspeed = yspeed;
	if(random == 0){
		piece->sprite.map = read_xpm(stairs, &tempwidth, &tempheight);
	}
	else if(random == 1){

	}
	else if(random == 2){

	}
	else if(random == 3){

	}
	else if(random == 4){

	}
	else if(random == 5){

	}
	else if(random == 6){

	}

	piece->sprite.width = tempwidth;
	piece->sprite.height = tempheight;

	return piece;
};

