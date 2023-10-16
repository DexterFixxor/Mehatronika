/*
 * led.c
 *
 *  Created on: Oct 16, 2023
 *      Author: dexter
 */

#include "led.h"
#include <stm32f4xx.h>


void LED_Init()
{
	// Ukljucivanje clock signala za PORTA
	RCC->AHB1ENR |= (1 << 0);

	//Ukljucimo PA5, odnosno Pin 5 na portu A, da radi u rezimu digitalnog izlaza
	GPIOA->MODER &= ~(0b11 << 5 * 2);
	GPIOA->MODER |= (0b01 << 5 * 2);
}
