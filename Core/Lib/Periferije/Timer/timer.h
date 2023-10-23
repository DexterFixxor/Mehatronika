/*
 * timer.h
 *
 *  Created on: Oct 16, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_TIMER_TIMER_H_
#define LIB_PERIFERIJE_TIMER_TIMER_H_

typedef struct
{
	unsigned char flg_100s : 1;
	unsigned char flg_90s : 1;
	unsigned char flg_LED : 1;

	unsigned char flg_timeoutStart : 1;
	unsigned char flg_timeoutEnd : 1;
}sTimerFlags_t;

volatile extern sTimerFlags_t timer_flags;
volatile extern unsigned int system_ms;
volatile extern unsigned int timeout_ms;

void Timer_Init();

void Timer_delay(unsigned int ms);
void Timer_timeout(unsigned int ms);

#endif /* LIB_PERIFERIJE_TIMER_TIMER_H_ */
