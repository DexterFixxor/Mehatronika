/*
 * timer.c
 *
 *  Created on: Oct 16, 2023
 *      Author: dexter
 */

#include "timer.h"
#include <stm32f4xx.h>

extern volatile unsigned int system_ms = 0;

void Timer_Init()
{
	// Ukljucimo clock signal za Timer2
	RCC->APB1ENR |= (0b1 << 0);

	//Podesavanje preskalera i timer_period-a
	// APB1 max 42 MHz, posto smo mi stavili max frekvenciju, na toliko nam radi i APB1

	TIM2->PSC = 42 - 1;
	TIM2->ARR = 1000 - 1;

	// Dozvola prekida brojaca
	TIM2->DIER |= (0b1 << 0);
	// Enable brojac
	TIM2->CR1 |= (0b1 << 0);

	NVIC->ISER[0] |= (1 << 28);
}

void Timer_delay(unsigned int ms)
{
	unsigned int tmp = system_ms;

	while((tmp + ms) > system_ms);
}

void Timer_timeout(unsigned int ms)
{

}

void TIM2_IRQHandler()
{
	if((TIM2->SR & (0b1 << 0)) == 0b1)
	{
		system_ms++;



		TIM2->SR &= ~(0b1 << 0);
	}
}
