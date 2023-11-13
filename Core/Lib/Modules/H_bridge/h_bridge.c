/*
 * h_bridge.c
 *
 *  Created on: Nov 9, 2023
 *      Author: dexter
 */


#include "h_bridge.h"
#include <stm32f4xx.h>


void HBridge_Init()
{
	/*
	 * PA0: EN1
	 *
	 * PA8: AIN1
	 * PA9: AIN2
	 */

	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOAEN_Pos);

	// output mode
	GPIOA->MODER &= ~(0b11 << 0 * 2 | 0b11 << 8 * 2 | 0b11 << 9 * 2);
	GPIOA->MODER |=  (0b01 << 0 * 2 | 0b01 << 8 * 2 | 0b01 << 9 * 2);

	// push-pull
	GPIOA->OTYPER &= ~(0b1 << 0 | 0b1 << 8 | 0b1 << 9);

	HBridge_Enable();
}

void HBridge_Enable()
{
	GPIOA->ODR |= (0b1 << 0);
}

void HBridge_SetDir(const eDirection_t dir)
{
	switch(dir)
	{
	case STOP:
		GPIOA->ODR &= ~(0b1 << 8 | 0b1 << 9);
		break;
	case CW:
		GPIOA->ODR |=  (0b1 << 8);
		GPIOA->ODR &= ~(0b1 << 9);
		break;
	case CCW:
		GPIOA->ODR &= ~(0b1 << 8);
		GPIOA->ODR |=  (0b1 << 9);
		break;
	}
}
