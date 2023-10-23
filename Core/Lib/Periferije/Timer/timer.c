/*
 * timer.c
 *
 *  Created on: Oct 16, 2023
 *      Author: dexter
 */
#include <stm32f4xx.h>

#include "timer.h"
#include "Moduli/Odometrija/odom.h"

volatile unsigned int system_ms = 0;
volatile unsigned int timeout_ms = 0;
volatile sTimerFlags_t timer_flags;

void Timer_Init()
{
	RCC->APB2ENR |= (0b1 << RCC_APB2ENR_TIM1EN_Pos);

		//Podesavanje preskalera i timer_period-a
		// APB2 max 84 MHz, posto smo mi stavili max frekvenciju, na toliko nam radi i APB1

		TIM1->PSC = 84 - 1;
		TIM1->ARR = 1000 - 1;

		// Dozvola prekida brojaca
		TIM1->DIER |= (0b1 << 0);
		TIM1->EGR |= (1 << 0);
		// Enable brojac
		TIM1->CR1 |= (0b1 << 0);
		TIM1->CR1 |= (1 << 2);

		while(! (TIM1->SR & (1 << 0)));

		NVIC->ISER[0] |= (1 << 25);
}

void Timer_delay(unsigned int ms)
{
	unsigned int tmp = system_ms;

	while((tmp + ms) > system_ms);
}

void Timer_timeout(unsigned int ms)
{
	if(timer_flags.flg_timeoutStart == 0)
		{
			timeout_ms = ms;
			timer_flags.flg_timeoutStart = 1;
			timer_flags.flg_timeoutEnd = 0;
		}
}

void TIM1_UP_TIM10_IRQHandler()
{
	if((TIM1->SR & (0b1 << 0)) == 0b1)
		{
			system_ms++;

			if (system_ms % 10 == 0)
				Odom_update(10);

			if(timer_flags.flg_timeoutStart == 1 && (timeout_ms > 0))
			{
				timeout_ms--;
			}
			if(timer_flags.flg_timeoutStart == 1 && timeout_ms == 0)
			{
				timer_flags.flg_timeoutEnd = 1;
				timer_flags.flg_timeoutStart = 0;
			}

			TIM1->SR &= ~(0b1 << 0);
		}
}
