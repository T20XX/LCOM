#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8254.h"
#include "i8042.h"

static int mouse_hook_id=2;

int mouse_subscribe_int() {
	int mouse_id = BIT(mouse_hook_id);

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id) != OK)
		return 1;
	if (sys_irqenable(&mouse_hook_id) != OK)
		return 1;
	return mouse_id;
}

int mouse_unsubscribe_int() {
	if (sys_irqrmpolicy(&mouse_hook_id) != OK)
		return 1;
	if (sys_irqdisable(&mouse_hook_id) != OK)
		return 1;
	return 0;
}

int mouse_output(){
	unsigned long stat, data;
	while( 1 ) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			return data;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int write_to_mouse(){
	unsigned long stat;

	//Write Byte to Mouse
	while( 1 ) {

		sys_inb(STAT_REG, &stat);
		if(( stat & IBF ) == 0) {
			if ( sys_outb(IN_BUF, BYTETOMOUSE) != OK )
				return 1;

			sys_inb(OUT_BUF, &stat);
			if(stat == ACK)
				return 0;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int enable_packets(){
	unsigned long stat;
	//Enable Sending Data Packets
	while( 1 ) {

		sys_inb(STAT_REG, &stat);
		if(( stat & IBF ) == 0) {
			if ( sys_outb(OUT_BUF, ENABLE_PACKETS ) != OK )
				return 1;

			sys_inb(OUT_BUF, &stat);
			if(stat == ACK)
				return 0;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

void print_packet(long int packet[3]){
	long int temp;

	//byte1:
	printf(" B1=0x%02x ", packet[0]);
	//byte2:

	printf(" B2=0x%02x ", packet[1]);

	//byte 3

	//LB
	printf(" B3=0x%02x ", packet[2]);
	temp = packet[0] & LB;
	printf(" LB=%d ", temp);

	//MB
	temp = packet[0] & MB;
	temp >>= 2;
	printf(" MB=%d ", temp);

	//RB
	temp = packet[0] & RB;
	temp >>= 1;
	printf(" RB=%d ", temp);

	//XOV
	temp = packet[0] & XOV;
	temp >>= 6;
	printf(" XOV=%d ", temp);

	//YOV
	temp = packet[0] & YOV;
	temp >>= 7;
	printf(" YOV=%d ", temp);

	//X
	temp = packet[1] - 256 *((packet[0] & XSIGN) >> 4);
	printf(" X=%d ",temp);

	//Y

	temp = packet[2] - 256 * ((packet[0] & YSIGN) >> 5);
	printf(" Y=%d\n", temp);
}

int status_request(){
	unsigned long stat;
	//Enable Sending status
	while( 1 ) {

		sys_inb(STAT_REG, &stat);
		if(( stat & IBF ) == 0) {
			if ( sys_outb(OUT_BUF, STATUS_REQUEST ) != OK )
				return 1;

			sys_inb(OUT_BUF, &stat);
			if(stat == ACK)
				return 0;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

void print_config(long int config[3]){
	long int temp;

	//RIGHT
	temp = config[0] & RIGHT;
	printf(" Right=%d\n", temp);

	//MIDDLE
	temp = config[0] & MIDDLE;
	temp >>= 2;
	printf(" Middle=%d\n", temp);

	//LEFT
	temp = config[0] & LEFT;
	temp >>= 1;
	printf(" Left=%d\n", temp);

	//SCALING
	temp = config[0] & SCALING;
	temp >>= 4;
	printf(" Scaling=%d\n", temp);

	//ENABLE
	temp = config[0] & ENABLE;
	temp >>= 5;
	printf(" Enable=%d\n", temp);

	//REMOTE
	temp = config[0] & REMOTE;
	temp >>= 6;
	printf(" Remote=%d\n", temp);

	//RESOLUTION
	temp = config[1] & RESOLUTION;
	printf(" Resolution=%d\n", temp);

	//SAMPLE RATE
	temp = config[2];
	printf(" Sample Rate=%d\n", temp);
}
