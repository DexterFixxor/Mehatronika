/*
 * timer.h
 *
 *  Created on: Oct 19, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIPHERALS_TIMER_TIMER_H_
#define LIB_PERIPHERALS_TIMER_TIMER_H_

#include <stm32f4xx.h>

typedef struct
{
	unsigned char flg_100s : 1;
	unsigned char flg_90s : 1;

	unsigned char flg_1s : 1;

	unsigned char flg_timeout_start : 1;
	unsigned char flg_timeout_end : 1;

}sTimerFlags_t;

volatile sTimerFlags_t timer_flags;
volatile extern unsigned int timeout_ms;
volatile extern unsigned int system_ms;

void TIMER_init();

void TIMER_delay(unsigned int ms);

void TIMER_timeout(unsigned int ms);

#endif /* LIB_PERIPHERALS_TIMER_TIMER_H_ */
