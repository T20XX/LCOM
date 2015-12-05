#include "menu.h"



Menu* new_main_menu(){
	Menu * main_menu = (Menu*) malloc(sizeof(Menu));
	main_menu->background = loadBitmap("/home/lcom/proj/code/img/test.bmp");

	unsigned int i;
	for(i = 0, i< 4;i++)
	{
		main_menu->buttons[i].x = 300;
		main_menu->buttons[i].y = 200 + i * 75;
		main_menu->buttons[i].width = 50;
		main_menu->buttons[i].height = 50;
	}
	return main_menu;
}

Menu * new_score_menu(){
	Menu * score_menu = (Menu*) malloc(sizeof(Menu));
	score_menu->background = loadBitmap("/home/lcom/proj/code/img/test.bmp");

	unsigned int i;
	score_menu->buttons[5].x = 300;
	score_menu->buttons[5].y =700;
	score_menu->buttons[5].width = 50;
	score_menu->buttons[5].height = 50;


	//tabelas de scores.

	return score_menu;
}

void draw_main_menu(Menu * menu){
	menu = new_main_menu();
}

void draw_score_menu(Menu * menu){
	menu = new_score_menu;
}

void update_main_menu(Menu * menu){

	if(menu->state = BUTTON1_ABOVE)
		menu->buttons[0].above;
	else if(menu->state == BUTTON2_ABOVE)
		menu->buttons[1].above;
	else if(menu->state ==  BUTTON3_ABOVE)
		menu->buttons[2].above;
	else if(menu->state == BUTTON4_ABOVE)
		menu->buttons[3].above;
	else if(menu->state == BUTTON5_ABOVE)
		menu->buttons[4].above;
	//else if(menu->state == BUTTON6_ABOVE)
	//	; voltar paratras? usado no scoremenu(BACK)

	if(menu->event == BUTTON1_CLICK)
		; // 1vs1 modo peças
	else if(menu->event == BUTTON2_CLICK)
		; //1vs1 modo tempo
	else if(menu->event == BUTTON3_CLICK)
		; // 1vs1 modo persona
	else if(menu->event == BUTTON4_CLICK)
		; //Scores
	else if(menu->event == BUTTON5_CLICK)
		;	//exit game
	//else if(menu->event == BUTTON6_CLICK)
	//	; voltar para tras? usado no score menu(BACK)


}

void update_score_menu(Menu * menu){

	if(menu->state == BUTTON6_ABOVE)
		menu->buttons[5].above;
	if(menu->event == BUTTON6_CLICK)
		; //volta para o main_menu; (BACK)
}
