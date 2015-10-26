#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"
#include "kbd.h"
#include "timer.h"

unsigned int code;

void kbd_int_handler() {
	code = kbd_output();
	if ((code & CODETYPE) == 0x00)
		printf("Makecode : 0x%02x \n",code);
	else
		printf("Breakcode : 0x%02x \n",code);
}

int kbd_test_scan(unsigned short ass) {
	kbd_subscribe_int();

		int irq_set = kbd_subscribe_int();
		int ipc_status;
		message msg;
		int r;

		while( code != BREAKCODE ) { /* You may want to use a different condition */
			/* Get a request message. */
			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
						kbd_int_handler();  /* process it */
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
				}
			} else { /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
			}
		}
		kbd_unsubscribe_int();
		return 0;
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
   unsigned int led_pos;
   unsigned int led[3];
   led[0]=0;
   led[1]=0;
   led[2]=0;
   int i;

   timer_subscribe_int();

   	int  irq_set = timer_subscribe_int();
   	int ipc_status;
   	message msg;
   	int r;

   	int counter = 0; //Inicialização do contador

   	while( counter < (n * 60)) { /* You may want to use a different condition */
   		/* Get a request message. */
   		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
   			printf("driver_receive failed with: %d", r);
   			continue;
   		}
   		if (is_ipc_notify(ipc_status)) { /* received notification */
   			switch (_ENDPOINT_P(msg.m_source)) {
   			case HARDWARE: /* hardware interrupt notification */
   				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
   					counter++;
   					if(counter % 60 == 0) //Imprimir apenas quando 60 ticks
   					{
   						led_pos = (counter/60 -1);
   						kbd_change_led(leds[led_pos],led);
   					}
   				}
   				break;
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

int kbd_test_timed_scan(unsigned short n) {
	 timer_subscribe_int();

	   	int  timer_irq_set = timer_subscribe_int();
	   	int  kbd_irq_set = kbd_subscribe_int();
	   	int ipc_status;
	   	message msg;
	   	int r;

	   	int counter = 0; //Inicialização do contador

	   	while( counter < (n * 60) && code != BREAKCODE) { /* You may want to use a different condition */
	   		/* Get a request message. */
	   		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
	   			printf("driver_receive failed with: %d", r);
	   			continue;
	   		}
	   		if (is_ipc_notify(ipc_status)) { /* received notification */
	   			switch (_ENDPOINT_P(msg.m_source)) {
	   			case HARDWARE: /* hardware interrupt notification */
	   				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
	   					counter = 0;
	   					kbd_int_handler();
		   				break;
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
	   	}
	   	timer_unsubscribe_int();
	   	kbd_unsubscribe_int();
	   	return 0;
}
