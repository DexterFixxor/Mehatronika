/*
 * timer.c
 *
 *  Created on: Oct 19, 2023
 *      Author: dexter
 */

#include "timer.h"

volatile unsigned int system_ms = 0;

volatile sTimerFlags_t timer_flags;
volatile unsigned int timeout_ms = 0;

void TIMER_init()
{
	//Config TIM2 to work on 1kHz

	// Enable TIM2 clock
	RCC->APB1ENR |= (0b1 << 0);

	//Set TIM2 prescaler and TIMER_PERIOD (ARR)
	// APB1 42MHz
	/*
	 * f_clk = 84 MHz
	 * f_timer = 1kHz
	 *
	 * PSC = 84  - 1
	 * TIMER_PERIOD = ?
	 *
	 *				f_clk
	 *	f_timer = ---------------------------
	 *				(PSC + 1) * TIMER_PERIOD
	 *
	 * TIMER_PERIOD = 1000 - 1
	 */
	TIM2->PSC = 84 -1;
	TIM2->ARR = 1000 - 1;

	// Enable TIM2 interrupt
	TIM2->DIER |= (0b1 << 0);
	TIM2->EGR |= (1 << 0);

	// Enable counter
	TIM2->CR1 |= (0b1 << 0);
	TIM2->CR1 |= (1 << 2); //interrupt only on overflow/underflow

	while(! (TIM2->SR & (1 << 0)));

	unsigned int vec_pos = 28;

	// TIM2 interrupt vector table: position 28
	NVIC->ISER[(int)(vec_pos/32)] |= (0b1 << (vec_pos % 32));
}

void TIMER_delay(unsigned int ms)
{
	unsigned int tmp_ms = system_ms;

	while ( (system_ms - tmp_ms) < ms);
}

void TIMER_timeout(unsigned int ms)
{
	if (timer_flags.flg_timeout_start == 0)
	{
		timer_flags.flg_timeout_start = 1;
		timer_flags.flg_timeout_end = 0;
		timeout_ms = ms;
	}
}

void TIM2_IRQHandler()
{
	if ( (TIM2->SR & (0b1 << 0)) == (0b1 << 0) )
	{
		// Code block
		system_ms++;

		if (timer_flags.flg_timeout_start == 1 && (timeout_ms > 0))
		{
			timeout_ms--;
		}
		else if(timer_flags.flg_timeout_start == 1 && timeout_ms == 0)
		{
			timer_flags.flg_timeout_end = 1;
			timer_flags.flg_timeout_start = 0;
		}

		// reset UIF bit
		TIM2->SR &= ~(0b1 << 0);

	}
}
