#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int counter;
int irq_set;
static int timer_hook_id = 0;
int timer0_id;

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char conf, tmp = 0x00, MSB, LSB;
	unsigned long div;
	timer_get_conf(timer,&conf);
	tmp = conf & BIT(0);
	tmp |= (TIMER_LSB_MSB | TIMER_SQR_WAVE);
	if (timer != 0)
	{
		if (timer = 1)
			tmp |= TIMER_SEL1;
		else if (timer = 2)
			tmp |= TIMER_SEL2;
		else return 1;
	}
	div = TIMER_FREQ/freq;
	LSB = div;
	MSB = div >> 8;
	if (sys_outb(TIMER_CTRL, tmp) != OK)
		return 1;
	if (sys_outb(TIMER_0+timer, LSB) != OK)
		return 1;
	if (sys_outb(TIMER_0+timer, MSB) != OK)
		return 1;
	return 0;
}

int timer_subscribe_int() {
	timer_hook_id = 0;
	timer0_id= BIT(timer_hook_id);
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
		return 1;
	if (sys_irqenable(&timer_hook_id) != OK)
		return 1;
	return timer0_id;
}

int timer_unsubscribe_int() {
	if (sys_irqrmpolicy(&timer_hook_id) != OK)
		return 1;
	if (sys_irqdisable(&timer_hook_id) != OK)
		return 1;
	return 0;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned char tempbyte;
	tempbyte = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
	if (sys_outb(TIMER_CTRL, tempbyte) != OK)
		return 1;
	unsigned long aux;
	if (sys_inb(TIMER_0+timer,&aux) != OK)
		return 1;
	*st = aux;
	return 0;
}

int timer_display_conf(unsigned char conf) {
	unsigned char tempbyte;
	unsigned char onebit;
	tempbyte = conf;
	int i;
    const char *a[8];
    a[0] = "BCD : ";
    a[1] = "Programmed Mode : ";
    a[2] = "Programmed Mode : ";
    a[3] = "Programmed Mode : ";
    a[4] = "Type Of Access : ";
    a[5] = "Type Of Access : ";
    a[6] = "Null Count : ";
    a[7] = "Output : ";

	    for(i = 7; 0 <= i; i --){
	    	onebit = (tempbyte >> i) & 0x01;
	    	printf ("%s", a[i]);
	    	printf("%d\n", onebit);
	    }
	        return 0;
}

int timer_test_square(unsigned long freq) {
	if (timer_set_square(0, freq))
		return 0;
	else
		return 1;
}

int timer_test_int(unsigned long n) {
	timer_subscribe_int();

	int  irq_set = BIT(timer0_id);
	int ipc_status;
	message msg;
	int r;

	counter = 0; //Inicialização do contador

	while( counter < (n * 60)) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();  /* process it */
					if(counter % 60 == 0); //Imprimir apenas quando 60 ticks
						//imprimir algo
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	timer_unsubscribe_int();
	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;
	timer_get_conf(timer,&conf);
	return timer_display_conf(conf);

}
