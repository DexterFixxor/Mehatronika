/*
 * uart.c
 *
 *  Created on: Nov 2, 2023
 *      Author: dexter
 */

#include <stm32f4xx.h>
#include "uart.h"

void UART1_Init()
{
	/*
	 * Half-duplex
	 * baud: 115200 Bps
	 * data: 1 start, 8 data, n stop bita
	 *
	 * PA9: UART1, Tx, AF7
	 * PA10: UART1, Rx, AF7
	 */

	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOAEN_Pos);
	RCC->APB2ENR |= (0b1 << RCC_APB2ENR_USART1EN_Pos);

	/* GPIOA PA9 i PA10 u alt. func AF7 */
	GPIOA->MODER &= ~(0b11 << 9 * 2 | 0b11 << 10 * 2);
	GPIOA->MODER |=  (0b10 << 9 * 2 | 0b10 << 10 * 2);

	/* AFR register na alt. funkciju 7, za pinove 9 i 10*/
	GPIOA->AFR[1] &= ~(0b1111 << 1 /* pin 9 */ * 4 | 0b1111 << 2 /* pin10*/ * 4);
	GPIOA->AFR[1] |=  (0b0111 << 1 * 4 | 0b0111 << 2 * 4);

	/* USART1 config */

	// OVER8, oversampling by 16
	USART1->CR1 &= ~(0b1 << 15);

	// 1 start, 8 data, n stop bits
	USART1->CR1 &= ~(0b1 << 12);

	// parity control disable
	USART1->CR1 &= ~(0b1 << 10);

	// interrupt enable on Rx data receive
//	USART1->CR1 |= (0b1 << 5);

	//enable Tx i Rx
	USART1->CR1 |= (0b1 << 3 | 0b1 << 2);

	// stop bits: 1 stop bit
	USART1->CR2 &= ~(0b11 << 12);

	//half-duplex mode
	USART1->CR3 |= (0b1 << 3);
	GPIOA->ODR |= (0b1 << 9); // open-drain, floating point


	USART1->BRR = (45 << 4 | 9);

	// Enable USART
	USART1->CR1 |= (0b1 << 13);

	// enable NVIC for USART1 ISR
//	NVIC->ISER[1] |= (0b1 << (37 % 32));
}


void UART1_SendChar(unsigned char ch)
{
	USART1->DR = ch;
	while(! (USART1->SR & (0b1 << 6)));
}

void UART1_SendArray(unsigned char* array, unsigned int size)
{
	for (int i=0; i < size; i++)
		UART1_SendChar(*array++);
}
