#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
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
	sys_inb(timer,&aux);
	*st = aux;
	return 0;
}

int timer_display_conf(unsigned char conf) {
	unsigned char tempbyte;
	printf("0x%x \n",conf);
	tempbyte = conf & TIMER_RB_SEL(-1);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(0);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(1);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(2);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(3);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(4);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(5);
	printf("0x%x \n",tempbyte);
	tempbyte = conf & TIMER_RB_SEL(6);
	printf("0x%x \n",tempbyte);
	return 0;
}

int timer_test_square(unsigned long freq) {
	
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
