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
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

void print_packet(){

	//byte1:
	printf("  \nB1=0x%02x  ", data_packet[0]);
	//byte2:

	printf("  B2=0x%02x  ", data_packet[1]);

	//byte 3
	printf("  B3=0x%02x  ",data_packet[2]);

	//LB
	prinft("  LB=%i  ", );

	//MB
	prinft("  MB=%i  ", );

	//RB
	prinft("  RB=%i  ", );

	//XOV
	prinft("  XOV=%i  ", );

	//YOV
	printf("  XOY=%i  ", );

	//X
	printf("  X=%i",);

	//Y
	printf("  Y=%i", );

}
