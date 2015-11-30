#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>
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

//typedef enum {MAIN_MENU, GAME_MODE1, GAME_MODE2,GAME_MODE3,GAME_MODE4,HIGH_SCORES,EXIT} state;

//typedef enum {CLICK_BUTTON1, CLICK_BUTTON2, CLICK_BUTTON3, CLICK_BUTTON4, CLICK_BUTTON5, CLICK_BUTTON6} main_menu_event;


typedef struct {
	unsigned int x, y;
} Position;

Position mouse_position = { .x = 512, .y = 384};

static long int packet[3];
static unsigned int packet_counter = 0 ;

unsigned long code;
bool is_two_byte = false;

int mainhandler(){
	vg_init (0x105);				//Initialization of graphics mode in 1024x768 resolution

	menu_handler();

	vg_exit();

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
		//Impressão da informação dos packets
		mouse_position.x += packet[1] - 256 *((packet[0] & XSIGN) >> 4);
		mouse_position.y -= packet[2] - 256 * ((packet[0] & YSIGN) >> 5);
	}
	return 0;
}


int menu_handler(){
	//só para testes
	Sprite mouse_sprite;
	mouse_sprite.map = read_xpm(cross, &mouse_sprite.width, &mouse_sprite.height);
	mouse_sprite.x = mouse_position.x;
	mouse_sprite.y = mouse_position.y;

	Bitmap *img = (Bitmap*) malloc(sizeof(Bitmap));;

	img = loadBitmap("/home/lcom/proj/img/test.bmp");

	if (img != NULL){

		int mouse_irq_set = mouse_subscribe_int();
		int timer_irq_set = timer_subscribe_int();
		int ipc_status;
		message msg;
		int r;
		write_to_mouse();
		enable_packets();
		int counter = 0; //Inicialização do contador

		while( counter < (10 * 60)) {
			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
						mouse_packet_handler();
					}
					if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
						counter++;
					}
				default:
					break; /* no other notifications expected: do nothing */
				}
			} else { /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
			}
			drawBitmap(img, 10, 10 , ALIGN_LEFT);
			mouse_sprite.x = mouse_position.x;
			mouse_sprite.y = mouse_position.y;
			vg_sprite(&mouse_sprite,0);
			vg_buffer();
		}

		mouse_unsubscribe_int();
		timer_unsubscribe_int();
	}
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
	/*if ((code & CODETYPE) == 0x00)
		printf("Makecode : 0x%02x \n",code);
	else
		printf("Breakcode : 0x%02x \n",code);*/
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

int game_handler(){
	Game *game;
	game = new_game(0);

	vg_sprite(&game->actual_piece->sprite,0);
	vg_sprite(&game->next_piece->sprite,0);
}
