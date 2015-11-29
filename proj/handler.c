#include "handler.h"
#include "game.h"

typedef enum {MAIN_MENU, GAME_MODE1, GAME_MODE2,GAME_MODE3,GAME_MODE4,HIGH_SCORES,EXIT} state;
typedef enum {CLICK_BUTTON1, CLICK_BUTTON2, CLICK_BUTTON3, CLICK_BUTTON4, CLICK_BUTTON5, CLICK_BUTTON6} event;

int mainhandler(){
	vg_init (0x105);				//Initialization of graphics mode in 1024x768 resolution

	Game *game;
	game = new_game(0);

	vg_exit ();

	return 0;
}
