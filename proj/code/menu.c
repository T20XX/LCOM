#include "menu.h"
#include "video_gr.h"

Button new_button(const char* filename_null,const char* filename_above){
	Button * button = (Button*) malloc(sizeof(Button));
	button->null = map_Bitmap(filename_null, &button->width, &button->height);
	button->above = map_Bitmap(filename_above, &button->width, &button->height);
	return *button;
}

Menu * new_main_menu(){
	Menu * main_menu = (Menu*) malloc(sizeof(Menu));
	main_menu->buttons = (Button*)malloc(sizeof(Button)*6);
	main_menu->background = loadBitmap("/home/lcom/proj/code/img/test.bmp");

	main_menu->buttons[0].null = map_Bitmap("/home/lcom/proj/code/img/button0_null.bmp", &main_menu->buttons[0].width, &main_menu->buttons[0].height);
	main_menu->buttons[0].above = map_Bitmap("/home/lcom/proj/code/img/button0_above.bmp", &main_menu->buttons[0].width, &main_menu->buttons[0].height);
	main_menu->buttons[1].null = map_Bitmap("/home/lcom/proj/code/img/button1_null.bmp", &main_menu->buttons[1].width, &main_menu->buttons[1].height);
	main_menu->buttons[1].above = map_Bitmap("/home/lcom/proj/code/img/button1_above.bmp", &main_menu->buttons[1].width, &main_menu->buttons[1].height);
	main_menu->buttons[2].null = map_Bitmap("/home/lcom/proj/code/img/button2_null.bmp", &main_menu->buttons[2].width, &main_menu->buttons[2].height);
	main_menu->buttons[2].above = map_Bitmap("/home/lcom/proj/code/img/button2_above.bmp", &main_menu->buttons[2].width, &main_menu->buttons[2].height);
	main_menu->buttons[3].null = map_Bitmap("/home/lcom/proj/code/img/button3_null.bmp", &main_menu->buttons[3].width, &main_menu->buttons[3].height);
	main_menu->buttons[3].above = map_Bitmap("/home/lcom/proj/code/img/button3_above.bmp", &main_menu->buttons[3].width, &main_menu->buttons[3].height);
	main_menu->buttons[4].null = map_Bitmap("/home/lcom/proj/code/img/button4_null.bmp", &main_menu->buttons[4].width, &main_menu->buttons[4].height);
	main_menu->buttons[4].above = map_Bitmap("/home/lcom/proj/code/img/button4_above.bmp", &main_menu->buttons[4].width, &main_menu->buttons[4].height);
	main_menu->buttons[5].null = map_Bitmap("/home/lcom/proj/code/img/button5_null.bmp", &main_menu->buttons[5].width, &main_menu->buttons[5].height);
	main_menu->buttons[5].above = map_Bitmap("/home/lcom/proj/code/img/button5_above.bmp", &main_menu->buttons[5].width, &main_menu->buttons[5].height);

	unsigned int i;
	for(i = 0; i < 6;i++)
	{
		//main_menu->buttons[i].width = 50;
		//main_menu->buttons[i].height = 50;
		main_menu->buttons[i].x = (getH_res()/2) - (main_menu->buttons[i].width/2);
		main_menu->buttons[i].y =200 + i * 75;
	}


	main_menu->event = NOACTION;
	main_menu->state = DO_NOTHING;

	return main_menu;
}

Menu * new_score_menu(){
	Menu * score_menu = (Menu*) malloc(sizeof(Menu));
	score_menu->background = loadBitmap("/home/lcom/proj/code/img/test.bmp");

	unsigned int i;
	score_menu->buttons[5].width = 50;
	score_menu->buttons[5].height = 50;
	score_menu->buttons[5].x = 300;
	score_menu->buttons[5].y =700;

	//tabelas de scores.

	return score_menu;
}

void draw_main_menu(Menu * menu){
	drawBitmap(menu->background,0,0,ALIGN_LEFT);
	unsigned int i;
	for(i = 0; i < 6; i++){
		if(menu->buttons[i].isAbove == 1){
			vg_map(menu->buttons[i].above, menu->buttons[i].x, menu->buttons[i].y, menu->buttons[i].width, menu->buttons[i].height);
		}
		else{
			vg_map(menu->buttons[i].null, menu->buttons[i].x, menu->buttons[i].y, menu->buttons[i].width, menu->buttons[i].height);
		}
	}
}

/*void draw_score_menu(Menu * menu){
	menu = new_score_menu;
}*/

void update_main_menu(Menu * menu){

	if(menu->state == BUTTON0_SELECT)
		menu->buttons[0].isAbove = 1;
	else if(menu->state == BUTTON1_SELECT)
		menu->buttons[1].isAbove = 1;
	else if(menu->state == BUTTON2_SELECT)
		menu->buttons[2].isAbove = 1;
	else if(menu->state == BUTTON3_SELECT)
		menu->buttons[3].isAbove = 1;
	else if(menu->state == BUTTON4_SELECT)
		menu->buttons[4].isAbove = 1;
	else if(menu->state == BUTTON5_SELECT)
		menu->buttons[5].isAbove = 1;
	else if(menu->state == DO_NOTHING){
		unsigned int i;
		for(i=0; i < 6; i++){
			menu->buttons[i].isAbove = 0;
		}
	}


	if(menu->state == SINGLEPLAYER)
		;
	else if(menu->state == MULTIPLAYER)
		; //1vs1 modo tempo
	else if(menu->state == MULTIPLAYER)
		; // 1vs1 modo persona
	else if(menu->state == HIGHSCORES)
		; //Scores
	else if(menu->state == EXIT)
		;	//exit game
	//else if(menu->event == BUTTON6_CLICK)
	//	; voltar para tras? usado no score menu(BACK)*/
}

void update_main_menu_state(Menu * menu){
	//menu->last_state = menu->state;

	if(menu->main_menu_event == LEFT_CLICK){
		if(menu->state == BUTTON0_SELECT)
			menu->state = SINGLEPLAYER;
		else if(menu->state == BUTTON1_SELECT)
			menu->state = MULTIPLAYER;
		else if(menu->state == BUTTON2_SELECT)
			menu->state = MULTIPLAYER;
		else if(menu->state == BUTTON3_SELECT)
			menu->state = HIGHSCORES;
		else if(menu->state == BUTTON4_SELECT)
			menu->state = EXIT;
		else if(menu->state == BUTTON5_SELECT)
			menu->state = EXIT; // O QUE E AQUI?
		else if(menu->state == DO_NOTHING)
			menu->state = DO_NOTHING;
	}
	else {
	if(menu->event == BUTTON0_ISABOVE)
		menu->state = BUTTON0_SELECT;
	if(menu->event == BUTTON1_ISABOVE)
		menu->state = BUTTON1_SELECT;
	if(menu->Event == BUTTON2_ISABOVE)
		menu->state = BUTTON2_SELECT;
	if(menu->event == BUTTON3_ISABOVE)
		menu->state = BUTTON3_SELECT;
	if(menu->event == BUTTON4_ISABOVE)
		menu->state = BUTTON4_SELECT;
	if(menu->event == BUTTON5_ISABOVE)
		menu->state = BUTTON5_SELECT;
	}
}


/*void update_score_menu(Menu * menu){

	if(menu->state == BUTTON6_ABOVE)
		menu->buttons[5].above;
	if(menu->event == BUTTON6_CLICK)
		; //volta para o main_menu; (BACK)
}*/

