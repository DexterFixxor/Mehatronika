/*
 * pwm.c
 *
 *  Created on: Nov 6, 2023
 *      Author: dexter
 */


#include "pwm.h"
#include <stm32f4xx.h>


void Motor1_PWM_Init()
{
	/*
	 * RCC TIM3
	 */
	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOBEN_Pos);
	RCC->APB1ENR |= (0b1 << RCC_APB1ENR_TIM3EN_Pos);
	// PB4, TIM3, CH1, AF3
	GPIOB->MODER &= ~(0b11 << 4 * 2);
	GPIOB->MODER |=  (0b10 << 4 * 2);

	GPIOB->AFR[0] &= ~(0b1111 << 4 * 4);
	GPIOB->AFR[0] |=  (2 << 4 * 4);

	/*
	 * TIM3, f_clk = 20kHz, CH1
	 */

	TIM3->PSC = 4 - 1; 		// 84MHz -> 21 MHz
	TIM3->ARR = MOTOR_ARR; 	// 21MHz ->  20kHz

	TIM3->CCMR1 &= ~(0b111 << 4);
	TIM3->CCMR1 |=  (0b110 << 4); // PWM Mode 1, output high when CNT < CCR1 (timer_period)

	TIM3->CCMR1 |= (0b1 << 3); // preload enable
	TIM3->CR1 |= (0b1 << 7); // auto-reload preload

	TIM3->EGR |= (0b1 << 0);

	TIM3->CCER |= (0b1 << 0);

	TIM3->CR1 |= (0b1 << 0);

}

void Motor1_SetPWM(unsigned int duc_inc)
{
	if (duc_inc < 0)
		duc_inc = 0;
	if (duc_inc > MOTOR_ARR)
		duc_inc = MOTOR_ARR;

	TIM3->CCR1 = duc_inc;

}

