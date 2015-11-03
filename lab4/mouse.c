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

int mouse_configure(){
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

void print_packet(){
	long int temp;

	//byte1:
	printf("  \nB1=0x%02x  ", packet[0]);
	//byte2:

	printf("  B2=0x%02x  ", packet[1]);

	//byte 3
	printf("  B3=0x%02x  ", packet[2]);
	temp = packet[0] & LB;
	//LB
	prinft("  LB=%i  ", temp);
	temp = packet[0] & MB;
	temp >> 2;
	//MB
	prinft("  MB=%i  ", temp);
	temp = packet[0] & RB;
	temp >> 1;
	//RB
	prinft("  RB=%i  ", temp);
	temp = packet[0] & XOV;
	temp >> 6;
	//XOV
	prinft("  XOV=%i  ", temp);
	temp = packet[0] & YOV;
	temp >> 7;
	//YOV
	printf("  YOV=%i  ", temp);

	//X
	printf("  X=%i",packet[2]);

	//Y
	printf("  Y=%i", packet[3]);
}
