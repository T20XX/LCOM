#ifndef __KBD_H
#define __KBD_H


int kbd_subscribe_int();

int kbd_unsubscribe_int();

int kbd_output();

int kbd_change_led(unsigned short nled, unsigned int *ledstate);

#endif
