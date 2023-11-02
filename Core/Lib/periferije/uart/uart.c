/*
 * uart.c
 *
 *  Created on: Nov 2, 2023
 *      Author: Andrej Čilag
 */

#include "uart.h"

#include "stm32f4xx.h"
#include <stdint.h>

volatile uint8_t input = 0;

static void
uart6_init ();

void
uart_init ()
{
  uart6_init ();
}

static void
uart6_init ()
{
  RCC->AHB1ENR |= (0b1 << 2); // Dozvola takta na GPIO Port-u C
  RCC->APB2ENR |= (0b1 << 5); // Dozvola takta na USART6 periferiji

  uint8_t const TX_PIN = 6;
  uint8_t const RX_PIN = 7;

  GPIOC->MODER &= ~(0b11 << TX_PIN * 2);
  GPIOC->MODER |= (0b10 << TX_PIN * 2);
  GPIOC->MODER &= ~(0b11 << RX_PIN * 2);
  GPIOC->MODER |= (0b10 << RX_PIN * 2);

  uint8_t const AF = 8;

  GPIOC->AFR[TX_PIN / 8] &= ~(0b1111 << (TX_PIN % 8) * 4);
  GPIOC->AFR[TX_PIN / 8] |= (AF << (TX_PIN % 8) * 4);
  GPIOC->AFR[RX_PIN / 8] &= ~(0b1111 << (RX_PIN % 8) * 4);
  GPIOC->AFR[RX_PIN / 8] |= (AF << (RX_PIN % 8) * 4);

  USART6->CR1 &= ~(0b1 << 12); // Dužina poruke
  USART6->CR2 &= ~(0b11 << 12); // Broj stop bitova

  // Baudrate = 9600
  //USART6->BRR = 0;
  USART6->BRR &= ~(0xFFFF);
  USART6->BRR |= ((546 << 4) | (14 << 0));

  // Uključivanje TX i RX pinova
  USART6->CR1 |= ((0b1 << 2) | (0b1 << 3));

  // Uključivanje prekida za prihvatanje poruke
  USART6->CR1 |= (0b1 << 5);

  // Odabir prekidne rutike koja se izvršava
  uint8_t const USART6_PREKID = 71;
  NVIC->ISER[USART6_PREKID / 32] |= (0b1 << USART6_PREKID % 32);

  // Uključivanje UART-a
  USART6->CR1 |= (0b1 << 13);
}

void
uart_send_byte (uint8_t podatak)
{
  USART6->DR = podatak;

  while (!(USART6->SR & (0b1 << 6)))
    {
      __NOP();
    }
  USART6->SR &= ~(0b1 << 6);
}

void
USART6_IRQHandler ()
{
  if (USART6->SR & (0b1 << 5))
    {
      input = USART6->DR;
    }
}
