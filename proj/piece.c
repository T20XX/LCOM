#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "piece.h"
#include "video_gr.h"

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
		piece->sprite.map = read_xpm(stairs, &tempwidth, &tempheight); // WARNING
	}
	else if(random == 1){
		piece->sprite.map = read_xpm(four, &tempwidth, &tempheight);
	}
	else if(random == 2){
		piece->sprite.map = read_xpm(square, &tempwidth, &tempheight);
	}
	else if(random == 3){
		piece->sprite.map = read_xpm(L, &tempwidth, &tempheight);
	}
	else if(random == 4){
		piece->sprite.map = read_xpm(L_reverse, &tempwidth, &tempheight);
	}
	else if(random == 5){
		piece->sprite.map = read_xpm(upstairs, &tempwidth, &tempheight);
	}
	else if(random == 6){
		piece->sprite.map = read_xpm(downstairs, &tempwidth, &tempheight);
	}

	piece->sprite.width = tempwidth;
	piece->sprite.height = tempheight;

	return piece;
};

void draw_piece(Piece * piece){
	vg_sprite(&piece->sprite, 0);
}
