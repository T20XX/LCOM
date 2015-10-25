#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8254.h"
#include "i8042.h"

static int kbd_hook_id=1;

int kbd_subscribe_int() {
	int kbd_id = BIT(kbd_hook_id);

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id) != OK)
		return 1;
	if (sys_irqenable(&kbd_hook_id) != OK)
		return 1;
	return kbd_id;
}

int kbd_unsubscribe_int() {
	if (sys_irqrmpolicy(&kbd_hook_id) != OK)
		return 1;
	if (sys_irqdisable(&kbd_hook_id) != OK)
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

int kbd_change_led(unsigned short nled, unsigned int *ledstate)
{
	unsigned long tmp;
	unsigned long com;
	unsigned int i;
	unsigned int led_pos=nled;

	sys_outb(OUT_BUF, CHANGELEDS);
	sys_inb(OUT_BUF, &tmp);

	for (i=0; i < 3; i++)
	{
		if (ledstate[i] == 1 && i != led_pos)
			com |= BIT(i);
	}

	if (ledstate[led_pos] == 0)
	{
		com |= BIT(led_pos);
		ledstate[led_pos]=1;
	}
	else if (ledstate[led_pos] == 1)
	{
		ledstate[led_pos]=0;
	}
	else return 1;

	sys_outb(OUT_BUF, com);
	return 0;

}
