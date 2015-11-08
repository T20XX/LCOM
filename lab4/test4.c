#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"
#include "timer.h"

static long int packet[3]; //Packet with 3 byte from mouse
static int counter = 0;
static unsigned int packet_counter = 0;
static long int config[3];
static unsigned int config_counter = 0;

int mouse_int_handler(){
	packet[packet_counter]=mouse_output();
	if (packet_counter == 0)
		if ((packet[packet_counter] & ISFIRSTPACKET) == 0)
			return;

	packet_counter ++;
	counter++;

	if(packet_counter == 3)
	{
		packet_counter = 0;
		print_packet(packet);
	}
	return 0;
}

int test_packet(unsigned short cnt){
	int irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	write_to_mouse();
	enable_packets();
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
   	int  timer_irq_set = timer_subscribe_int();
   	int  mouse_irq_set = mouse_subscribe_int();
   	int ipc_status;
   	message msg;
   	int r;

   	int counter = 0; //Inicialização do contador
   	write_to_mouse();
   	enable_packets();

   	while( counter < (idle_time * 60)) { /* You may want to use a different condition */
   		/* Get a request message. */
   		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
   			printf("driver_receive failed with: %d", r);
   			continue;
   		}
   		if (is_ipc_notify(ipc_status)) { /* received notification */
   			switch (_ENDPOINT_P(msg.m_source)) {
   			case HARDWARE: /* hardware interrupt notification */
   				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
   					counter = 0;
   					mouse_int_handler();
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
   	mouse_unsubscribe_int();
   	return 0;
}

int mouse_int_config_handler(){
	config[config_counter]=mouse_output();

	config_counter ++;

	if(config_counter == 3)
	{
		print_config(config);
	}
	return 0;
}

int test_config(void) {
	int irq_set = mouse_subscribe_int();
		int ipc_status;
		message msg;
		int r;
		write_to_mouse();
		status_request();
		while(config_counter < 3) { /* You may want to use a different condition */
			/* Get a request message. */
			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) {
						mouse_int_config_handler();  /* process it */
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

int test_gesture(short length, unsigned short tolerance) {
	/* To be completed ... */
}
