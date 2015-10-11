#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char conf;
	unsigned char tmp;
	timer_get_conf(0,&conf);
	conf &= (BIT(0) |  BIT(1) | BIT(2) | BIT(3));
	tmp = conf;
	tmp |= (BIT(5) | BIT (4));
	/*if (timer != 0)
	{
		tmp |= (BIT(5+timer));
	}*/
	sys_outb(TIMER_CTRL, tmp);
	sys_outb(TIMER_0, TIMER_FREQ/freq);
	return 0;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned char tempbyte;
	tempbyte = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
	sys_outb(TIMER_CTRL, tempbyte);
	unsigned long aux;
	sys_inb(TIMER_0+timer,&aux);
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
	return timer_set_square(0, freq);
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;
	timer_get_conf(timer,&conf);
	return timer_display_conf(conf);

}
