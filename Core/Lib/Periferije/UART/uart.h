/*
 * uart.h
 *
 *  Created on: Oct 30, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_UART_UART_H_
#define LIB_PERIFERIJE_UART_UART_H_

#define BUFF_SIZE 64

typedef struct
{
	unsigned int head;
	unsigned int tail;
	unsigned char data[BUFF_SIZE];
}sRingBuffer_t;

extern volatile sRingBuffer_t ringBuffer;


void UART2_Init();

void USART2_SendChar(unsigned char ch);
void USART2_SendString(unsigned char* str);
void USART2_ReadMsg(unsigned char* data, unsigned int *size);
void USART2_EchoMsg();

void USART2_SendArray(unsigned char* arr, unsigned int size);

#endif /* LIB_PERIFERIJE_UART_UART_H_ */
