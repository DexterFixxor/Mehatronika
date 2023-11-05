/*
 * uart.h
 *
 *  Created on: Nov 2, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_UART_UART_H_
#define LIB_PERIFERIJE_UART_UART_H_

void UART1_Init();

void UART1_SendChar(unsigned char ch);
void UART1_SendArray(unsigned char* array, unsigned int size);

#endif /* LIB_PERIFERIJE_UART_UART_H_ */
