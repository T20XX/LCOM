#include "game.h"
#include "piece.h"

typedef struct {
	unsigned int x, y;
} Position;

Position oneplayer_init = {.x= 500, .y = 60};			//Initial position for 1 player game mode
Position twoplayer_init = {.x= 100, .y = 60};			//Initial position for 2 player game mode

int yspeed_init = 1;					//Initial falling speed (y axis)

Game * new_game(unsigned int mode){
	Game * game = (Game*) malloc(sizeof(Game));

	if (mode == 0){				//SINGLEPLAYER MODE
		game->actual_piece = new_piece(oneplayer_init.x, oneplayer_init.y, yspeed_init);
		game->next_piece = new_piece(oneplayer_init.x+200, oneplayer_init.y, yspeed_init);
	}
	else if (mode == 1 || mode == 2){						//2 PLAYERS MODES

		game->actual_piece = new_piece(twoplayer_init.x, twoplayer_init.y, yspeed_init);
		game->next_piece = new_piece(twoplayer_init.x+200, twoplayer_init.y, yspeed_init);
	}

	return game;
}

void update_game(){

}



