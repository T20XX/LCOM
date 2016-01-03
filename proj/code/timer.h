/**
 * This module contains all functions to manage timer
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight:
 */
#ifndef __TIMER_H
#define __TIMER_H

int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Subscribes the timer interrupts
 *
 * @return Return timer ID to verify the interrupts and -1 upon failure
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes the timer interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int timer_unsubscribe_int();

int timer_get_conf(unsigned long timer, unsigned char *st);

int timer_test_square(unsigned long freq);

#endif /* __TIMER_H */
