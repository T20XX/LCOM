#include "game.h"
#include "video_gr.h"
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
	game->fall_delay = 40;
	game->timer_event = NO_TICK;
	game->kbd_event = NOKEY;
	game->state = DO_NOTHING;
	return game;
}

void update_gamestate(Game * game){
	game->last_state = game->state;
	 if (game->kbd_event == NOKEY){
		 game->state = DO_NOTHING;
	 }else if (game->kbd_event == LEFTKEY_DOWN){
		 game->state = MOVE_LEFT;
	 }else if (game->kbd_event == UPKEY_DOWN){
		 game->state = ROTATE;
	 }else if (game->kbd_event == RIGHTKEY_DOWN){
		 game->state = MOVE_RIGHT;
	 }else if (game->kbd_event == DOWNKEY_DOWN || game->timer_event == FALL_TICK){
		 game->state = FALL;
	 }else if (game->kbd_event == SPACEBAR_DOWN){
		 game->state = SWAP_PIECES;
	 }
}

void update_game(Game * game){
	if (game->state == FALL){
		game->actual_piece->sprite.yspeed += FACE_LENGTH;

	} else if (game->state == MOVE_LEFT){
		game->actual_piece->sprite.xspeed -= FACE_LENGTH;

	} else if (game->state == MOVE_RIGHT){
		game->actual_piece->sprite.xspeed += FACE_LENGTH;

	} else if (game->state == ROTATE){


	} else if (game->state == DO_NOTHING){

	} else if (game->state == SWAP_PIECES){


	} else if (game->state == REACH_END){

	}

	game->actual_piece->sprite.x += game->actual_piece->sprite.xspeed;
	game->actual_piece->sprite.y += game->actual_piece->sprite.yspeed;

	game->actual_piece->sprite.xspeed = 0;
	game->actual_piece->sprite.yspeed = 0;
}

void draw_game(Game * game){
	vg_sprite(&game->actual_piece->sprite,0);
	//vg_sprite(&game->next_piece->sprite,0);
}
