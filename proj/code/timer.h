#ifndef __TIMER_H
#define __TIMER_H

int timer_set_square(unsigned long timer, unsigned long freq);

int timer_subscribe_int(void );

int timer_unsubscribe_int();

int timer_get_conf(unsigned long timer, unsigned char *st);

int timer_test_square(unsigned long freq);

#endif /* __TIMER_H */
