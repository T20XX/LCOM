#ifndef __MENU_H
#define __MENU_H
#include "bitmap.h"
typedef enum main_menu_event {BUTTON1_CLICK, BUTTON2_CLICK, BUTTON3_CLICK, BUTTON4_CLICK, BUTTON5_CLICK, BUTTON6_CLICK} main_menu_event;
typedef enum main_menu_state {BUTTON1_ABOVE, BUTTON2_ABOVE, BUTTON3_ABOVE, BUTTON4_ABOVE, BUTTON5_ABOVE, BUTTON6_ABOVE} main_menu_state;

typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  uint16_t* above, null;           /**< the sprite pixmap (use read_xpm()) */
} Button;
//typedef enum {} mouse_game_event;

typedef struct {
	Bitmap* background;
	main_menu_state state;
	main_menu_event event;
	Button buttons[6];
} Menu;

Menu* new_main_menu();
Menu * new_score_menu();
void draw_main_menu(Menu * menu);
void draw_score_menu(Menu * menu);
void update_main_menu(Menu * menu);
void update_score_menu(Menu * menu)
#endif /* __MENU_H */
