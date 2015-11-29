#include "piece.h"
#include "sprite.h"
#incude ""


typedef struct{
	sprite sprite;
} piece;

piece *new_piece(int yspeed int xi, int yi){
	int random = rand() % 6;
	if(random == 0){
		sprite.map = read_xpm(stairs, &sprite.width,&sprite.height);
		sprite.x = xi;
		sprite.y = yi;
		sprite.xspeed = 0;
		sprite.yspeed = yspeed;
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
};

