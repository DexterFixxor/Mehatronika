/*
 * servo.c
 *
 *  Created on: Oct 30, 2023
 *      Author: dexter
 */

#include "servo.h"
#include <stm32f4xx.h>

void PWM_ServoInit()
{
	/*
	 * PB8, TIM10 CH1, AF3
	 * 50 Hz
	 */

	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOBEN_Pos);
	RCC->APB2ENR |= (0b1 << RCC_APB2ENR_TIM10EN_Pos);

	// GPIOB PB8 alt. fun.
	GPIOB->MODER &= ~(0b11 << 8 * 2);
	GPIOB->MODER |=  (0b10 << 8 * 2);

	GPIOB->AFR[1] &= ~(0b1111 << 0 * 4);
	GPIOB->AFR[1] |=  (0b0011 << 0 * 4);

	// TIM10 CH1 pwm mode

	// 50 Hz
	TIM10->ARR = 4000 - 1;
	TIM10->PSC = 420 - 1;

	TIM10->CCMR1 &= ~(0b111 << 4);
	TIM10->CCMR1 |=  (0b110 << 4); // PWM Mode 1, output high when CNT < CCR1 (timer_period)

	TIM10->CCMR1 |= (0b1 << 3); // preload enable
	TIM10->CR1 |= (0b1 << 7); // auto-reload preload

	TIM10->EGR |= (0b1 << 0);

	TIM10->CCER |= (0b1 << 0);

	TIM10->CR1 |= (0b1 << 0);


}

