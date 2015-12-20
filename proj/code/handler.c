#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>
#include "menu.h"
#include "i8042.h"
#include "i8254.h"
#include "handler.h"
#include "game.h"
#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "video_gr.h"
#include "piece.h"
#include "sprite.h"
#include "bitmap.h"
#include "vbe.h"


Mouse_t mouse;

static long int packet[3];
static unsigned int packet_counter = 0 ;

unsigned long code;
bool is_two_byte = false;

int timer_irq_set;
int kbd_irq_set;
int mouse_irq_set;

unsigned int selecao = 0;

int mainhandler(){
	vg_init (GRAPHICS_MODE_1024_768_256);				//Initialization of graphics mode in 1024x768 resolution
	timer_irq_set = timer_subscribe_int();
	kbd_irq_set = kbd_subscribe_int();
	mouse_irq_set = mouse_subscribe_int();
	write_to_mouse();
	enable_packets();

	mouse.map=map_Bitmap("/home/lcom/proj/code/img/mouse.bmp", &mouse.width, &mouse.height);
	while (selecao != 6){
		menu_handler();

		switch (selecao){
		case 1:
			game_handler();
			selecao = 0;
		case 2:
			selecao = 0;
		case 3:
			selecao = 0;
		case 4:
			selecao = 0;
		case 5:
			selecao = 0;

		}
		//game_handler();
		//selecao = 0;
	}

	vg_exit();
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	mouse_unsubscribe_int();



	return 0;
}

int mouse_packet_handler(){
	packet[packet_counter] = mouse_output();
	if (packet_counter == 0)
		//Verifica se é o primeiro packet através da verificação do bit 3 (grande probabilidade)
		if ((packet[packet_counter] & ISFIRSTPACKET) == 0)
			return;

	//incrementa as variáveis referentes ao contador de packets e ao geral
	packet_counter ++;

	if(packet_counter == 3)
	{
		//Reset da variável contador dos packets
		packet_counter = 0;

		mouse.deltax = packet[1] - 256 *((packet[0] & XSIGN) >> 4);
		mouse.deltay = packet[2] - 256 * ((packet[0] & YSIGN) >> 5);

		if (mouse.x + mouse.deltax > 0 && mouse.x + mouse.deltax + mouse.width < getH_res() &&
				mouse.y - mouse.deltay > 0 && mouse.y -mouse.deltay + mouse.height< getV_res()){
			mouse.x += mouse.deltax;
			mouse.y -= mouse.deltay;
		}
		mouse.left_btn = packet[0] & LB;
		mouse.middle_btn = packet[0] & MB;
		mouse.right_btn = packet[0] & RB;


	}
	return 0;
}

main_menu_event main_menu_event_handler(Menu* menu){

	if(mouse.left_btn == 1)
		return LEFT_CLICK;
	else {
		if(mouse.x < menu->buttons[0].x || mouse.x > (menu->buttons[0].x + menu->buttons[0].width))
			return NOACTION;
		else{
			unsigned int i, index = 0;
			for(i = 0; i < 6; i++){
				if(mouse.y >= menu->buttons[i].y && mouse.y <= (menu->buttons[i].y+ menu->buttons[i].height))
					break;
				index++;
			}
			if(index == 0)
				return BUTTON0_ISABOVE;
			else if(index == 1)
				return BUTTON1_ISABOVE;
			else if(index == 2)
				return BUTTON2_ISABOVE;
			else if(index == 3)
				return BUTTON3_ISABOVE;
			else if(index == 4)
				return BUTTON4_ISABOVE;
			else if(index == 5)
				return BUTTON5_ISABOVE;
			else return NOACTION;
		}
	}
}

selecao_handler(Menu* menu){
	if (menu->state == SINGLEPLAYER)
		selecao = 1;
	if (menu->state == MULTIPLAYER)
		selecao = 2;
	if (menu->state == CHARACTERS)
		selecao = 3;
	if (menu->state == HIGHSCORES)
		selecao = 4;
	if (menu->state == INSTRUCTIONS)
		selecao = 5;
	if (menu->state == SHUTDOWN)
		selecao = 6;
}


int menu_handler(){

	Menu * main_menu = new_main_menu();

	int ipc_status;
	message msg;
	int r;
	int counter = 0; //Inicialização do contador

	while(selecao == 0) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
					mouse_packet_handler(mouse);
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					main_menu->event = main_menu_event_handler(main_menu);
					update_main_menu_state(main_menu);
					update_main_menu(main_menu);
					draw_main_menu(main_menu);
					vg_map_transparent(mouse.map, mouse.x, mouse.y, mouse.width, mouse.height, 0);
					vg_string("""TESTE, 1, 2, 3", 100,100,2,0xffff);
					//vg_pixel(mouse_position.x,mouse_position.y,20);
					vg_buffer();
					selecao_handler(main_menu);
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	delete_main_menu(main_menu);
	return 0;
}


int kbd_int_handler() {
	code = kbd_output();
	if (code == 0xE0)
	{
		is_two_byte= true;
		return 0;
	}
	else if (is_two_byte)
	{
		code |= (0xE0 << 8);
		is_two_byte = false;
	}
}


kbd_game_event kbd_event_handler(){
	if(code == UPKEY_MC)
	{
		return UPKEY_DOWN;
	}
	else if(code == DOWNKEY_MC)
	{
		return DOWNKEY_DOWN;
	}
	else if(code == RIGHTKEY_MC)
	{
		return RIGHTKEY_DOWN;
	}
	else if(code == LEFTKEY_MC)
	{
		return LEFTKEY_DOWN;
	}
	else if(code == SPACEBAR_MC)
	{
		return SPACEBAR_DOWN;
	}
	else return NOKEY;
}

mouse_game_event mouse_event_handler(){
//	if(mouse.deltax > 10)
//	{
//		return MOUSE_LEFT;
//	}
//	else if(mouse.deltax < 10)
//	{
//		return MOUSE_RIGHT;
//	}
//	else
		return MOUSE_STOPPED;
}

timer_game_event timer_event_handler(int counter){
	if( counter%40 == 0)
		return FALL_TICK;
	else return NO_TICK;
}


int game_handler(){
	Game *game;
	game = new_game(0);


	int ipc_status;
	message msg;
	int r;
	int counter = 0; //Inicialização do contador

	while( code != BREAKCODE && game->state != GAME_OVER) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();
					if (packet_counter == 0){
						game->mouse_event = mouse_event_handler();
						update_gamestate(game);
						update_game(game);
					}
				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
					game->last_kbd_event = game->kbd_event;
					game->kbd_event = kbd_event_handler();
					update_gamestate(game);
					update_game(game);
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					if (counter > 1000){
						counter = 0;
					}
					game->timer_event = timer_event_handler(counter);
					if(game->timer_event != NO_TICK){
						update_gamestate(game);
						update_game(game);
					}
					draw_game(game);
					char buffer[10];
					sprintf(buffer, "%d", counter);
					//vg_rectangle(0,0,200,100,BLACK);
					vg_string(buffer,0,0,2,WHITE);

					vg_buffer();
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	return 0;
}
