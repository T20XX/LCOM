#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"
#include "mouse.h"

void mouse_int_handler(){

}

int test_packet(unsigned short cnt){
	int irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r,i;
	int count = cnt;

	for(i = 0; i<count; i++ ) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_int_handler();  /* process it */
					}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	mouse_unsubscribe_int();
	return 0;
}	
	
int test_async(unsigned short idle_time) {
    /* To be completed ... */
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
