#include "menu.h"
#include "video_gr.c"


Menu * new_main_menu(){
	Menu * main_menu = (Menu*) malloc(sizeof(Menu));
	main_menu->buttons = malloc(sizeof(Button)*6);
	main_menu->background = loadBitmap("/home/lcom/proj/code/img/test.bmp");

	unsigned int i;
	for(i = 0; i < 6;i++)
	{
		//main_menu->buttons[i].width = 50;
		//main_menu->buttons[i].height = 50;
		main_menu->buttons[i].x = getH_res()/2 - (main_menu->buttons[i].width/2);
		main_menu->buttons[i].y = (getV_res()/2 - (main_menu->buttons[i].height)/2) + i * 75;
	}

	main_menu->buttons[0].null = map_Bitmap("/home/lcom/proj/code/img/button0_null", &main_menu->buttons[0].width, &main_menu->buttons[0].height);
	main_menu->buttons[0].above = map_Bitmap("/home/lcom/proj/code/img/button0_above", &main_menu->buttons[0].width, &main_menu->buttons[0].height);
	main_menu->buttons[1].null = map_Bitmap("/home/lcom/proj/code/img/button1_null", &main_menu->buttons[1].width, &main_menu->buttons[1].height);
	main_menu->buttons[1].above = map_Bitmap("/home/lcom/proj/code/img/button1_above", &main_menu->buttons[1].width, &main_menu->buttons[1].height);
	main_menu->buttons[2].null = map_Bitmap("/home/lcom/proj/code/img/button2_null", &main_menu->buttons[2].width, &main_menu->buttons[2].height);
	main_menu->buttons[2].above = map_Bitmap("/home/lcom/proj/code/img/button2_above", &main_menu->buttons[2].width, &main_menu->buttons[2].height);
	main_menu->buttons[3].null = map_Bitmap("/home/lcom/proj/code/img/button3_null", &main_menu->buttons[3].width, &main_menu->buttons[3].height);
	main_menu->buttons[3].above = map_Bitmap("/home/lcom/proj/code/img/button3_above", &main_menu->buttons[3].width, &main_menu->buttons[3].height);
	main_menu->buttons[4].null = map_Bitmap("/home/lcom/proj/code/img/button4_null", &main_menu->buttons[4].width, &main_menu->buttons[4].height);
	main_menu->buttons[4].above = map_Bitmap("/home/lcom/proj/code/img/button4_above", &main_menu->buttons[4].width, &main_menu->buttons[4].height);
	main_menu->buttons[5].null = map_Bitmap("/home/lcom/proj/code/img/button5_null", &main_menu->buttons[5].width, &main_menu->buttons[5].height);
	main_menu->buttons[5].above = map_Bitmap("/home/lcom/proj/code/img/button5_above", &main_menu->buttons[5].width, &main_menu->buttons[5].height);

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
	vg_map(menu->background, 0, 0, getH_res(), getV_res());
	unsigned int i;
	for(i = 0; i < 6; i++){
		if(buttons[i].isAbove){
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

	if(menu->state = BUTTON1_ABOVE)
		menu->buttons[0].isAbove = 1;
	else if(menu->state == BUTTON2_ABOVE)
		menu->buttons[1].isAbove = 1;
	else if(menu->state ==  BUTTON3_ABOVE)
		menu->buttons[2].isAbove = 1;
	else if(menu->state == BUTTON4_ABOVE)
		menu->buttons[3].isAbove = 1;
	else if(menu->state == BUTTON5_ABOVE)
		menu->buttons[4].isAbove = 1;


	if(menu->event == BUTTON1_CLICK)
		; // 1vs1 modo pe�as
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

void update_main_menu_state(Menu * menu, unsigned int mouse_x, unsigned int mouse_y){
	if(mouse_x < menu->buttons[0].x || mouse_x > (menu->buttons[0].x + menu->buttons[0].width))
		menu->state = NOACTION;
	else{
		unsigned int i, index = 0;
		for(i = 0; i <= 5; i++){
			if(menu->buttons[i].isAbove == 1)
				menu->buttons[i].isAbove = 0;
			if(mouse_y > menu->buttons[i].y && mouse_y < (menu->buttons[i].y+ menu->buttons[i].height))	break;
			index++;
		}
		if(index == 0)
			menu->state = BUTTON1_ABOVE;
		else if(index == 1)
			menu->state = BUTTON2_ABOVE;
		else if(index == 2)
			menu->state = BUTTON3_ABOVE;
		else if(index = 3)
			menu->state = BUTTON4_ABOVE;
		else if(index = 4)
			menu->state = BUTTON5_ABOVE;
		else if(index = 5)
			menu->state = NOACTION;
	}
}


/*void update_score_menu(Menu * menu){

	if(menu->state == BUTTON6_ABOVE)
		menu->buttons[5].above;
	if(menu->event == BUTTON6_CLICK)
		; //volta para o main_menu; (BACK)
}*/

