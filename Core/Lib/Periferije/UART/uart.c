/*
 * uart.c
 *
 *  Created on: Oct 30, 2023
 *      Author: dexter
 */

#include "uart.h"
#include <stm32f4xx.h>

volatile sRingBuffer_t ringBuffer = {
		.data = {0},
		.head = 0,
		.tail = 0
};

void UART2_Init()
{
	/*
	 * PA2 -> Tx
	 * PA3 -> Rx
	 *
	 * AF7 alt. fun.
	 */

	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOAEN_Pos);
	RCC->APB1ENR |= (0b1 << RCC_APB1ENR_USART2EN_Pos);

	/* PA2 i PA3 na AF7 */
	GPIOA->MODER &= ~(0b11 << 2 * 2 | 0b11 << 3 * 2);
	GPIOA->MODER |=  (0b10 << 2 * 2 | 0b10 << 3 * 2);

	GPIOA->AFR[0] &= ~(0b1111 << 2 * 4 | 0b1111 << 3 * 4);
	GPIOA->AFR[0] |=  (0b0111 << 2 * 4 | 0b0111 << 3 * 4);

	// Config UART2
	/*
	 * Baud: 115200 Bps
	 * start, 8bit, stop (no parity)
	 */

	USART2->CR1 |=  (0b1 << 13); // USART Enable
	USART2->CR1 &= ~(0b1 << 12); // 1 start, 8 data, n stop bits
	USART2->CR1 &= ~(0b1 << 11); // wakeup on idle line
	USART2->CR1 &= ~(0b1 << 10); // parity disabled

	USART2->CR1 |=  (0b1 << 5);	 // Enable RX interrupt

	USART2->CR1 |=  (0b1 << 3 | 0b1 << 2); //enable TX i RX

	USART2->CR3 |= (0b1 << 3); // half-duplex

	// 115200 baudrate
	/*
	 * BRR (baud rate register)
	 * DIV_Fraction = 12
	 * DIV_Mantissa = 22
	 */

	USART2->BRR = (22 << 4 | 12);

	// Nested Vectored Interrupt Controller (NVIC)
	NVIC->ISER[(int)(38/32)] |= (0b1 << (38 % 32));

}

void USART2_SendChar(unsigned char ch)
{
	while(!((USART2->SR & (0b1 << 6)) == (0b1 << 6)));
	USART2->DR = ch;
}

void USART2_SendString(unsigned char* str)
{
	while(*str != '\0')
		USART2_SendChar(*str++);
}

void USART2_ReadMsg(unsigned char* data, unsigned int *size)
{
	*size = 0;
	while (ringBuffer.head != ringBuffer.tail)
	{
		*data = ringBuffer.data[ringBuffer.tail];
		ringBuffer.tail = (ringBuffer.tail + 1) % BUFF_SIZE;
		*(++data) = '\0';
		*size += 1;
	}
}

void USART2_EchoMsg()
{
	unsigned char data[BUFF_SIZE] = {0};
	unsigned int size;
	if (ringBuffer.head != ringBuffer.tail)
	{
		HAL_Delay(10);
		USART2_ReadMsg(data, &size);
		USART2_SendString(data);
	}

}

void USART2_SendArray(unsigned char* arr, unsigned int size)
{
	for (int i = 0; i < size; i++)
	{
		USART2_SendChar(*arr++);
	}
}

void USART2_IRQHandler()
{
	ringBuffer.data[ringBuffer.head] = USART2->DR;

	ringBuffer.head = (ringBuffer.head + 1) % BUFF_SIZE;

	// opciono
	ringBuffer.data[ringBuffer.head] = '\0';
}
