#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "vbe.h"
#include "test5.h"
#include "timer.h"

void *test_init(unsigned short mode, unsigned short delay) {
	int  timer_irq_set = timer_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	timer_test_square(60); //for�a o timer a trabalhar a 60HZ
	int counter = 0; //Inicializa��o do contador

	while( counter < (delay * 60)) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					if (counter >= (delay * 60))
						/*imprimir VRAM ADRESS */
						printf("Acabou o tempo");
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	timer_unsubscribe_int();
	return 0;

}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	/* To be completed */

}

int test_line(unsigned short xi, unsigned short yi, 
		unsigned short xf, unsigned short yf, unsigned long color) {

	/* To be completed */

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	/* To be completed */

}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}					

int test_controller() {

	/* To be completed */

}					

