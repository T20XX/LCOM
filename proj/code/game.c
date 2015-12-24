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
	game->actual_piece->sprite.x -= (int)((game->actual_piece->sprite.width / 2) /FACE_LENGTH) * 30;  //error when using face_length

	//Initializing others variables of game
	game->fall_delay = INIT_FALL_DELAY;
	game->timer_event = NO_TICK;
	game->kbd_event = NOKEY;
	game->mouse_event = MOUSE_STOPPED;
	game->state = DO_NOTHING;
	game->pieces_already_swapped = 0;
	game->points = 0;
	game->lines = 0;
	return game;
}

void update_gamestate(Game * game){
	//game->last_state = game->state;
	switch (game->kbd_event){

	case NOKEY:
		game->state = DO_NOTHING;
		break;

	case LEFTKEY_DOWN:
		if (can_piece_move_x(game->actual_piece,&game->board,0)== 0)
			game->state = MOVE_LEFT;
		else
			game->state = DO_NOTHING;
		break;

	case RIGHTKEY_DOWN:
		if (can_piece_move_x(game->actual_piece,&game->board,1)== 0)
			game->state = MOVE_RIGHT;
		else
			game->state = DO_NOTHING;
		break;

	case DOWNKEY_DOWN:
		if (can_piece_fall(game->actual_piece,&game->board)== 0)
			game->state = FALL;
		else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
			game->state = REACH_END;
		else
			game->state = GAME_OVER;
		break;

	case UPKEY_DOWN:
		game->state = ROTATE;
		break;

	case SPACEBAR_DOWN:
		if (game->pieces_already_swapped == 0){
			game->state = SWAP_PIECES;
			game->pieces_already_swapped = 1;
		}
		break;

	default:
		break;
	}
	//	if (game->kbd_event == DOWNKEY_DOWN || game->timer_event == FALL_TICK){
	//			if (can_piece_fall(game->actual_piece,&game->board)== 0)
	//				game->state = FALL;
	//			else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
	//				game->state = REACH_END;
	//			else
	//				game->state = GAME_OVER;
	//	}else if (game->kbd_event == NOKEY && game->mouse_event == MOUSE_STOPPED){
	//		game->state = DO_NOTHING;
	//
	//	}else if (game->kbd_event == LEFTKEY_DOWN || game->mouse_event == MOUSE_LEFT){
	//		if (can_piece_move_x(game->actual_piece,&game->board,0)== 0)
	//			game->state = MOVE_LEFT;
	//		else
	//			game->state = DO_NOTHING;
	//
	//	}else if (game->kbd_event == UPKEY_DOWN){
	//		game->state = ROTATE;
	//
	//	}else if (game->kbd_event == RIGHTKEY_DOWN || game->mouse_event == MOUSE_RIGHT){
	//		if (can_piece_move_x(game->actual_piece,&game->board,1)== 0)
	//			game->state = MOVE_RIGHT;
	//		else
	//			game->state = DO_NOTHING;
	//
	//	} else if (game->kbd_event == SPACEBAR_DOWN){
	//		if (game->pieces_already_swapped == 0){
	//			game->state = SWAP_PIECES;
	//			game->pieces_already_swapped = 1;
	//		}
	//	}

	if (game->timer_event == FALL_TICK){
		if(game->state != FALL){
			if (can_piece_fall(game->actual_piece,&game->board)== 0)
				game->state = FALL;
			else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
				game->state = REACH_END;
			else
				game->state = GAME_OVER;
		}
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
		Piece * rotated = (Piece*) malloc(sizeof(Piece));

		rotate_piece(game->actual_piece, &game->board);

		//delete_piece(game->actual_piece);
		//game->actual_piece = rotated;


	} else if (game->state == DO_NOTHING){

	} else if (game->state == SWAP_PIECES){
		swap_pieces (game->actual_piece,game->next_piece);

		//delete_piece(&temp);

		/*unsigned int * x = game->actual_piece->sprite.x;
		game->actual_piece->sprite.x = game->next_piece->sprite.x;
		game->next_piece->sprite.x = x;

		unsigned int * y = &game->actual_piece->sprite.y;
		&game->actual_piece->sprite.y = &game->next_piece->sprite.y;
		&game->next_piece->sprite.y = y;*/


	} else if (game->state == REACH_END){
		unsigned int lines_removed;

		add_piece(game->actual_piece, &game->board);
		lines_removed = remove_finished_lines(&game->board);
		game->actual_piece = game->next_piece;
		game->actual_piece->sprite.x = game->board.x + BOARD_RELATIVE_MIDDLE_X;
		game->actual_piece->sprite.y = game->board.y;

		game->next_piece = new_piece((game->board.x + 420),
				(game->board.y + 90));									//substituir por constante

		game->pieces_already_swapped = 0;
		game->points += 50;
		game->points += (lines_removed * 250);
		game->lines += lines_removed;
	}

	game->actual_piece->sprite.x += game->actual_piece->sprite.xspeed;
	game->actual_piece->sprite.y += game->actual_piece->sprite.yspeed;

	game->actual_piece->sprite.xspeed = 0;
	game->actual_piece->sprite.yspeed = 0;
}

void draw_game(Game * game){
	char temp[5];
	vg_rectangle(0,0,1024,768,BLACK);
	vg_map(game->board.map,game->board.x,game->board.y,game->board.width,game->board.height);
	vg_sprite(&game->actual_piece->sprite,0);

	vg_string("NEXT",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_NEXT_STRING_Y, 2, WHITE);
	vg_sprite(&game->next_piece->sprite,0);
	vg_string("POINTS",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_POINTS_STRING_Y, 2, WHITE);
	sprintf(temp, "%d", game->points);
	vg_string(temp,game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_POINTS_Y, 2, WHITE);
	vg_string("LINES",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_LINES_STRING_Y, 2, WHITE);
	sprintf(temp, "%d", game->lines);
	vg_string(temp,game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_LINES_Y, 2, WHITE);
}

void delete_game(Game * game){
	free(game);
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

int remove_finished_lines(Board * board){
	int lines_removed = 0;

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
			lines_removed++;
		} else if(counter == 0){
			break;
		} else{
			board_ptr -= (board->width * 30);				//??
		}
		counter = 0;
	}
	return lines_removed;
}

void rotate_piece(Piece * piece, Board * board){
	/*Piece temp = *piece;
	uint16_t * temp_ptr = temp.sprite.map;
	uint16_t * piece_ptr = piece->sprite.map;*/

	/*rotated->sprite.x = piece->sprite.x;
	rotated->sprite.y = piece->sprite.y;
	rotated->sprite.height = piece->sprite.width;
	rotated->sprite.width = piece->sprite.height;*/

	//*rotated = *piece;

	uint16_t * temp_ptr;
	temp_ptr = (uint16_t *)malloc(piece->sprite.height*piece->sprite.width * 2);
	uint16_t * piece_ptr = piece->sprite.map;

	unsigned int i,j;
	for(i = 0; i < piece->sprite.width; i++){
		piece_ptr = piece->sprite.map +(piece->sprite.width * (piece->sprite.height-1))+i;
		for(j=0; j < piece->sprite.height;j++){
			*temp_ptr = *piece_ptr;
			temp_ptr++;
			piece_ptr -= piece->sprite.width;
		}
	}

	unsigned int width = piece->sprite.width;
	unsigned int height = piece->sprite.height;

	piece->sprite.height = width;
	piece->sprite.width = height;

	piece_ptr = piece->sprite.map;
	temp_ptr -= (width * height);
	piece->sprite.map = temp_ptr;

	if (can_piece_be_placed(piece, board) == 1){
		piece->sprite.height = height;
		piece->sprite.width = width;
		piece->sprite.map = piece_ptr;
		//free (temp_ptr);
	}
	else
	{
		//free (piece_ptr);
	}

	//	rotated->sprite.x = piece->sprite.x;
	//		rotated->sprite.y = piece->sprite.y;
	//	rotated->sprite.height = piece->sprite.width;
	//	rotated->sprite.width = piece->sprite.height;
	//
	//	rotated->sprite.map = temp_ptr - (rotated->sprite.width*rotated->sprite.height);

	//piece->sprite.width = temp.sprite.height;
	//piece->sprite.height = temp.sprite.width;
	//piece->sprite.map =  temp.sprite.map;

	//delete_piece(piece);
}

void swap_pieces(Piece * actual, Piece * next){
	unsigned int actual_x = actual->sprite.x;
	unsigned int next_x = next->sprite.x;
	unsigned int actual_y = actual->sprite.y;
	unsigned int next_y = next->sprite.y;
	unsigned int width = actual->sprite.width;
	unsigned int height = actual->sprite.height;

	uint16_t * temp_ptr;
	temp_ptr = (uint16_t *)malloc(actual->sprite.height*actual->sprite.width * 2);
	uint16_t *actual_ptr = actual->sprite.map;

	unsigned int i;
	for(i=0; i < actual->sprite.height;i++){
		memcpy(temp_ptr,actual_ptr,actual->sprite.width*2);
		temp_ptr += actual->sprite.width;
		actual_ptr += actual->sprite.width;
	}

	actual->sprite = next->sprite;
	actual->sprite.x=actual_x;
	actual->sprite.y=actual_y;

	next->sprite.width = width;
	next->sprite.height = height;
	next->sprite.map = temp_ptr - (width*height);
}
