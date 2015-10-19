#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"

static int hook_id=1;

int kbd_subscribe_int() {
	int mask = BIT(hook_id);

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE, &hook_id) != OK)
		return 1;
	if (sys_irqenable(&hook_id) != OK)
		return 1;
	return 0;
}

int timer_unsubscribe_int() {
	if (sys_irqrmpolicy(&hook_id) != OK)
		return 1;
	if (sys_irqdisable(&hook_id) != OK)
		return 1;
	return 0;
}
