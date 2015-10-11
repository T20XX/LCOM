#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char conf, tmp, MSB, LSB;
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
	LSB = div & 0xFF;
	MSB = div >> 8;
	sys_outb(TIMER_CTRL, tmp);
	sys_outb(TIMER_0+timer, LSB);
	sys_outb(TIMER_0+timer, MSB);
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
	if (timer_set_square(0, freq))
		return 0;
	else
		return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;
	timer_get_conf(timer,&conf);
	return timer_display_conf(conf);

}
