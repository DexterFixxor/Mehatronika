/*
 * h_bridge.c
 *
 *  Created on: Nov 6, 2023
 *      Author: dexter
 */

#include "h_bridge.h"
#include "Periferije/PWM/pwm.h"
#include <stm32f4xx.h>

void Motor1_HBridge_Init()
{
	/*
	 * EN1: 	PA0
	 * AIN1: 	PA8
	 * AIN2: 	PA9
	 */
	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOAEN_Pos);

	GPIOA->MODER &= ~(0b11 << 0 * 2 | 0b11 << 8 * 2 | 0b11 << 9 * 2);
	GPIOA->MODER |=  (0b01 << 0 * 2 | 0b01 << 8 * 2 | 0b01 << 9 * 2);

	GPIOA->OTYPER &= ~(0b1 << 0 | 0b1 << 8 | 0b1 << 9);

}

void Motor1_Enable()
{
	GPIOA->ODR |= (0b1 << 0);
}

void Motor1_Disable()
{
	GPIOA->ODR &= ~(0b1 << 0);
}

void Motor1_Direction(eMotorDir_t dir)
{

	switch(dir)
	{
	case STOP:
		GPIOA->ODR |= (0b1 << 8);
		GPIOA->ODR |= (0b1 << 9);
		break;
	case CW:
		GPIOA->ODR |= (0b1 << 8);
		GPIOA->ODR &= ~(0b1 << 9);
		break;
	case CCW:
		GPIOA->ODR &= ~(0b1 << 8);
		GPIOA->ODR |= (0b1 << 9);
		break;
	}
}
/*
 * duc: Duty Cycle in range [-1,1]
 */
void Motor1_SetDuC(float duc)
{
	float duc_corrected = duc;
	if (duc < 0)
	{
		duc_corrected *= -1;
		Motor1_Direction(CW);
	}
	else if (duc > 0)
	{
		Motor1_Direction(CCW);
	}
	else
	{
		Motor1_Direction(STOP);
	}

	Motor1_SetPWM((unsigned int)(duc_corrected * MOTOR_ARR));
}
