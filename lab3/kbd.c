#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8254.h"
#include "i8042.h"

static int hook_id=1;

int kbd_subscribe_int() {
	int kbd_id = BIT(hook_id);

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id) != OK)
		return 1;
	if (sys_irqenable(&hook_id) != OK)
		return 1;
	return kbd_id;
}

int kbd_unsubscribe_int() {
	if (sys_irqrmpolicy(&hook_id) != OK)
		return 1;
	if (sys_irqdisable(&hook_id) != OK)
		return 1;
	return 0;
}

int kbd_output(){
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
