/*
 * pwm.c
 *
 *  Created on: Nov 9, 2023
 *      Author: dexter
 */

#include "pwm.h"
#include <stm32f4xx.h>

void Motor1_PWM_Init()
{
	/*
	 * PB4: AF2 -> TIM3;CH1;
	 *
	 * F_pwm = 20 kHz
	 */

	/* RCC GPIOB, TIM3 */
	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOBEN_Pos);
	RCC->APB1ENR |= (0b1 << RCC_APB1ENR_TIM3EN_Pos);

	/* GPIOB, PB4-> AF2 */

	GPIOB->MODER &= ~(0b11 << 4 * 2);
	GPIOB->MODER |=  (0b10 << 4 * 2);

	GPIOB->AFR[0] &= ~(0b1111 << 4 * 4);
	GPIOB->AFR[0] |=  (0b0010 << 4 * 4); //AF2

	/* TIM3 */

	TIM3->PSC = PWM_PSC;
	TIM3->ARR = PWM_ARR;

	// PWM Mode 1, bits OC1M[6:4] = 0b110
	TIM3->CCMR1 &= ~(0b111 << 4);
	TIM3->CCMR1 |=  (0b110 << 4);

	// Output compare preload enable, CH1, bit OC1PE[3] = 0b1
	TIM3->CCMR1 |= (0b1 << 3);

	// set PWM duty cycle on first PWM period
	TIM3->CCR1 = 0UL;
	// Update generation
	TIM3->EGR |= (0b1 << 0);

	// Capture/compare 1 output enable
	TIM3->CCER |= (0b1 << 0);

	TIM3->CR1 |= (0b1 << 7);

	// counter enable
	TIM3->CR1 |= (0b1 << 0);
}

void Motor1_PWM_Duc(unsigned int duc)
{
	if (duc > (PWM_ARR))
	{
		TIM3->CCR1 = PWM_ARR;
	}
	else if(duc < 0)
	{
		TIM3->CCR1 = 0;
	}
	else
	{
		TIM3->CCR1 = duc;
	}
}
