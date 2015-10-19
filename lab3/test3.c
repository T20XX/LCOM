#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "kbd.c"

void kbd_int_handler() {

}

int kbd_test_scan(unsigned short ass) {
	kbd_subscribe_int();

	int  irq_set = BIT(timer0_id);
	int ipc_status;
	message msg;
	int r;

	while( 1 ) { /* You may want to use a different condition */
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
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
