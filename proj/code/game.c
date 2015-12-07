#include <stdint.h>
#include "game.h"
#include "video_gr.h"
#include "logic.h"
#include "bitmap.h"

//typedef struct {
//	unsigned int x, y;
//} Position;
//
//Position oneplayer_init = {.x= 500, .y = 60};			//Initial position for 1 player game mode
//Position twoplayer_init = {.x= 100, .y = 60};			//Initial position for 2 player game mode

Game * new_game(unsigned int mode){
	Game * game = (Game*) malloc(sizeof(Game));
	game->game_mode = mode;

	unsigned int x_board, y_board, x_actual,y_actual,x_next,y_next;

	if (game->game_mode == 0){				//SINGLEPLAYER MODE
		x_board = ONE_PLAYER_BOARD_X;
		y_board = ONE_PLAYER_BOARD_Y;

		x_actual= x_board + BOARD_RELATIVE_MIDDLE_X;
		y_actual= y_board;
		x_next = x_board + ONE_PLAYER_RELATIVE_NEXT_PIECE_X;
		y_next = y_board + ONE_PLAYER_RELATIVE_NEXT_PIECE_Y;
	}
	else if (game->game_mode == 1 || game->game_mode == 2){						//2 PLAYERS MODES
		x_board = TWO_PLAYER_BOARD_X;
		y_board = TWO_PLAYER_BOARD_Y;

		x_actual= x_board + BOARD_RELATIVE_MIDDLE_X;
		y_actual= y_board;
		x_next = x_board + TWO_PLAYER_RELATIVE_NEXT_PIECE_X;
		y_next = y_board + TWO_PLAYER_RELATIVE_NEXT_PIECE_Y;
	}

	//Creating board
	game->board.map = map_Bitmap("/home/lcom/proj/code/img/board.bmp", &game->board.width, &game->board.height);
	game->board.x = x_board;
	game->board.y = y_board;

	//Creating first two pieces
	game->actual_piece = new_piece(x_actual,y_actual);
	game->next_piece = new_piece(x_next,y_next);

	//Correction in x to place piece in middle after knowing its width
	game->actual_piece->sprite.x -= (int)((game->actual_piece->sprite.width / 2) /30) * 30;  //error when using face_length

	//Initializing others variables of game
	game->time_elapsed = 0;
	game->fall_delay = INIT_FALL_DELAY;
	game->timer_event = NO_TICK;
	game->kbd_event = NOKEY;
	game->state = DO_NOTHING;
	game->pieces_already_swapped = 0;
	return game;
}

void update_gamestate(Game * game){
	game->last_state = game->state;
	if (game->kbd_event == NOKEY){
		game->state = DO_NOTHING;

	}else if (game->kbd_event == LEFTKEY_DOWN){
		if (can_piece_move_x(game->actual_piece,&game->board,0)== 0)
			game->state = MOVE_LEFT;
		else
			game->state = DO_NOTHING;

	}else if (game->kbd_event == UPKEY_DOWN){
		game->state = ROTATE;

	}else if (game->kbd_event == RIGHTKEY_DOWN){
		if (can_piece_move_x(game->actual_piece,&game->board,1)== 0)
			game->state = MOVE_RIGHT;
		else
			game->state = DO_NOTHING;

	}else if (game->kbd_event == DOWNKEY_DOWN){
		if (can_piece_fall(game->actual_piece,&game->board)== 0)
			game->state = FALL;
		else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
			game->state = REACH_END;
		else
			game->state = GAME_OVER;

	}else if (game->kbd_event == SPACEBAR_DOWN){
		if (game->pieces_already_swapped == 0){
			game->state = SWAP_PIECES;
			game->pieces_already_swapped = 1;
		}
	}

	if (game->timer_event == FALL_TICK){
		if (can_piece_fall(game->actual_piece,&game->board)== 0)
			game->state = FALL;
		else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
			game->state = REACH_END;
		else
			game->state = GAME_OVER;

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
		rotate_piece(game->actual_piece);

	} else if (game->state == DO_NOTHING){

	} else if (game->state == SWAP_PIECES){
		Piece temp = *game->actual_piece;
		*game->actual_piece = *game->next_piece;
		*game->next_piece = temp;
		delete_piece(&temp);

		/*unsigned int * x = game->actual_piece->sprite.x;
		game->actual_piece->sprite.x = game->next_piece->sprite.x;
		game->next_piece->sprite.x = x;

		unsigned int * y = &game->actual_piece->sprite.y;
		&game->actual_piece->sprite.y = &game->next_piece->sprite.y;
		&game->next_piece->sprite.y = y;*/


	} else if (game->state == REACH_END){
		add_piece(game->actual_piece, &game->board);
		remove_finished_lines(&game->board);
		game->actual_piece = game->next_piece;
		game->actual_piece->sprite.x = game->board.x + BOARD_RELATIVE_MIDDLE_X;
		game->actual_piece->sprite.y = game->board.y;

		game->next_piece = new_piece((game->board.x ),
				(game->board.y));
	}

	game->actual_piece->sprite.x += game->actual_piece->sprite.xspeed;
	game->actual_piece->sprite.y += game->actual_piece->sprite.yspeed;

	game->actual_piece->sprite.xspeed = 0;
	game->actual_piece->sprite.yspeed = 0;

	game->pieces_already_swapped = 0;
}

void draw_game(Game * game){
	vg_map(game->board.map,game->board.x,game->board.y,game->board.width,game->board.height);
	vg_sprite(&game->actual_piece->sprite,0);
	vg_sprite(&game->next_piece->sprite,0);
}

void add_piece(Piece * piece, Board * board){
	uint16_t * piece_ptr = piece->sprite.map;
	uint16_t * board_ptr = board->map;

	board_ptr += (piece->sprite.x - board->x) + (board->width * (piece->sprite.y - board->y));

	unsigned int i,j;
	for(i = 0; i< piece->sprite.height; i++){
		for(j = 0; j < piece->sprite.width;j++){
			if ((*piece_ptr) != 0){
				*board_ptr = *piece_ptr;
			}
			piece_ptr++;
			board_ptr++;
		}
		board_ptr += (board->width - piece->sprite.width);
	}
}

void remove_finished_lines(Board * board){
	uint16_t * board_ptr = board->map;

	board_ptr += (board->width * 30 * 21) + 30;				//??

	unsigned int i,j, counter;
	for (i = 20; i > 0; i--){
		for (j = 0; j< 10; j++){
			if (*(board_ptr + j *30) != 0)
				counter++;
		}
		if(counter == 10){
			uint16_t * tmp_ptr = board_ptr - (board->width * 30);

			unsigned int lines_above;
			for(lines_above = 0; lines_above < i; lines_above++){
				for (j = 0; j< 30; j++){
					memcpy(board_ptr - (lines_above * board->width*30) + (j*board->width),tmp_ptr - (lines_above * board->width*30) + (j*board->width), 10*30*2);	//??
				}
			}
			i++;

		} else if(counter == 0){
			break;
		} else{
			board_ptr -= (board->width * 30);				//??
		}
		counter = 0;
	}
}

void rotate_piece(Piece * piece){
	Piece temp = *piece;
	uint16_t * temp_ptr = temp.sprite.map;
	uint16_t * piece_ptr = piece->sprite.map;

	unsigned int i,j;
	for(i = 0; i < piece->sprite.width; i++){
		piece_ptr = piece->sprite.map+i;
		for(j=0; j < piece->sprite.height;j++){
			*temp_ptr = *piece_ptr;
			temp_ptr++;
			piece_ptr += piece->sprite.width;
		}
	}

	piece->sprite.width = temp.sprite.height;
	piece->sprite.height = temp.sprite.width;
	piece->sprite.map =  temp.sprite.map;

	delete_piece(&temp);
}
