#include "game.h"
#include "piece.h"
#include "logic.h"

typedef struct {
	unsigned int x, y;
} Position;

Position oneplayer_init = {.x= 500, .y = 60};			//Initial position for 1 player game mode
Position twoplayer_init = {.x= 100, .y = 60};			//Initial position for 2 player game mode

int yspeed_init = 1;					//Initial falling speed (y axis)

Game * new_game(unsigned int mode){
	Game * game = (Game*) malloc(sizeof(Game));
	game->game_mode = mode;

	if (game->game_mode == 0){				//SINGLEPLAYER MODE
		game->actual_piece = new_piece(oneplayer_init.x, oneplayer_init.y, yspeed_init);
		game->next_piece = new_piece(oneplayer_init.x+200, oneplayer_init.y, yspeed_init);
	}
	else if (game->game_mode == 1 || game->game_mode == 2){						//2 PLAYERS MODES

		game->actual_piece = new_piece(twoplayer_init.x, twoplayer_init.y, yspeed_init);
		game->next_piece = new_piece(twoplayer_init.x+200, twoplayer_init.y, yspeed_init);
	}

	game->time_elapsed = 0;
	game->kbd_event = NOKEY;
	game->state = FALL;
	return game;
}

void update_gamestate(Game * game){
	last_state = state;
	 if (game->kbd_event == NOKEY){
		 game->state = FALL;
	 }else if (game->kbd_event == LEFTKEY_DOWN){
		 game->state = MOVE_LEFT;
	 }else if (game->kbd_event == UPKEY_DOWN){
		 game->state = ROTATE;
	 }else if (game->kbd_event == RIGHTKEY_DOWN){
		 game->state = MOVE_RIGHT;
	 }else if (game->kbd_event == DOWNKEY_DOWN){
		 game->state = FALL_QUICKER;
	 }else if (game->kbd_event == SPACEBAR_DOWN){
		 game->state = SWAP_PIECES;
	 }
}

void update_game(Game * game){
	if (game->state == FALL){


	} else if (game->state == MOVE_LEFT){


	} else if (game->state == MOVE_RIGHT){


	} else if (game->state == ROTATE){


	} else if (game->state == FALL_QUICKER){


	} else if (game->state == SWAP_PIECES){


	} else if (game->state == REACH_END){

	}
}

