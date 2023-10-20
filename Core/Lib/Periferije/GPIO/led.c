/*
 * led.c
 *
 *  Created on: Oct 19, 2023
 *      Author: dexter
 */

#include "led.h"

void LED_init()
{
	/*
	 * GPIOA port clock enable
	 */
	RCC->AHB1ENR |= (1 << 0);

	/*
	 * Reset GPIOA pin 5 na 00
	 */
//	GPIOA->MODER &= ~(GPIO_MODER_MODE5_Msk);
	GPIOA->MODER &= ~(0b11 << 5 * 2);

	// Set GPIOA pin 5 to general purpose output mode
	GPIOA->MODER |= (0b01 << 5 * 2);

	// set to push-pull
	GPIOA->OTYPER &= ~(0b1 << 5);
}
