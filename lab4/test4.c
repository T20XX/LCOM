#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"
#include "mouse.h"

static long int packet[3]; //Packet with 3 byte from mouse
static int counter = 0;

int mouse_int_handler(unsigned int packet_counter){
	packet[packet_counter]=mouse_output();
	if (packet_counter == 0)
		if ((data_packet[counter_mouse] & ISFIRSTPACKET) == 0)
			return;

	packet_counter ++;
	counter++;

	if(packet_counter == 3)
	{
		packet_mouse = 0;
		print_packet();
	}
	return 0;
}

int test_packet(unsigned short cnt){
	int irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r,i;
	unsigned int packet_counter = 0;

	while(counter < (cnt*3)) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_int_handler(packet_counter);  /* process it */
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
