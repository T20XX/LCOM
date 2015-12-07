#ifndef __MENU_H
#define __MENU_H
#include "bitmap.h"
#include <stdint.h>

typedef enum main_menu_event {lEFT_CLICK, BUTTON0_ISABOVE, BUTTON1_ISABOVE, BUTTON2_ISABOVE, BUTTON3_ISABOVE, BUTTON4_ISABOVE, BUTTON5_ISABOVE, NOACTION} main_menu_event;

typedef enum main_menu_state {BUTTON0_ABOVE, BUTTON1_ABOVE, BUTTON2_ABOVE, BUTTON3_ABOVE, BUTTON4_ABOVE, BUTTON5_ABOVE} main_menu_state;

typedef struct {
	unsigned int x,y;
	unsigned int width, height;
	uint16_t * above;
	uint16_t * null;
	unsigned int isAbove;
} Button;


typedef struct {
	Bitmap* background;
	main_menu_state state, last_state;
	main_menu_event event;
	Button * buttons;
} Menu;


Button new_button(const char* filename_null,const char* filename_above);
Menu * new_main_menu();
Menu * new_score_menu();
void draw_main_menu(Menu * menu);
void draw_score_menu(Menu * menu);
void update_main_menu(Menu * menu);
void update_score_menu(Menu * menu);
void update_main_menu_state(Menu * menu);
#endif /* __MENU_H */

