#ifndef __MENU_H
#define __MENU_H
#include "bitmap.h"
#include <stdint.h>

typedef enum main_menu_event {BUTTON0_CLICK, BUTTON1_CLICK, BUTTON2_CLICK, BUTTON3_CLICK, BUTTON4_CLICK, BUTTON5_CLICK} main_menu_event;
typedef enum main_menu_state {BUTTON0_ABOVE, BUTTON1_ABOVE, BUTTON2_ABOVE, BUTTON3_ABOVE, BUTTON4_ABOVE, BUTTON5_ABOVE, NOACTION} main_menu_state;

typedef struct {
  unsigned int x,y;
  unsigned int width, height;
  uint16_t * above;
  uint16_t * null;
  unsigned int isAbove;
} Button;


typedef struct {
	Bitmap* background;
	main_menu_state state;
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
void update_main_menu_state(Menu * menu, unsigned int mouse_x, unsigned int mouse_y);
#endif /* __MENU_H */

