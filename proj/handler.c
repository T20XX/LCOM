#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "handler.h"
#include "game.h"
#include "timer.h"
#include "kbd.h"
#include "mouse.h"

//typedef enum {MAIN_MENU, GAME_MODE1, GAME_MODE2,GAME_MODE3,GAME_MODE4,HIGH_SCORES,EXIT} state;
//typedef enum {CLICK_BUTTON1, CLICK_BUTTON2, CLICK_BUTTON3, CLICK_BUTTON4, CLICK_BUTTON5, CLICK_BUTTON6} event;

int mainhandler(){
	vg_init (0x105);				//Initialization of graphics mode in 1024x768 resolution

	Game *game;
	game = new_game(0);

	vg_sprite(game->actual_piece);

	int  timer_irq_set = timer_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	timer_test_square(60); //força o timer a trabalhar a 60HZ
	int counter = 0; //Inicialização do contador

	while( counter < (5 * 60)) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	timer_unsubscribe_int();

	vg_exit ();

	return 0;
}
