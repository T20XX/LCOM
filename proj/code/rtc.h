#ifndef __RTC_H
#define __RTC_H

#define RTC_REG_A 			10
#define RTC_REG_B  			11
#define RTC_REG_C  			12
#define RTC_REG_D  			13

#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REG 		0x71

#define RTC_SECONDS 		0
#define RTC_SECONDS_ALARM 	1
#define RTC_MINUTES			2
#define RTC_MINUTES_ALARM	3
#define RTC_HOURS 			4
#define RTC_HOURS_ALARM 	5
#define RTC_DAY_WEEK 		6
#define RTC_DAY_MONTH		7
#define RTC_MONTH			8
#define RTC_YEAR			9

#define RTC_IRQ				8

int rtc_subscribe_int();

int rtc_unsubscribe_int();

int rtc_current_time(unsigned long *hour, unsigned long *min, unsigned long *sec);

void rtc_draw_current_time(unsigned int x, unsigned int y);

#endif /* __RTC_H */
