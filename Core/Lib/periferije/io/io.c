/*
 * io.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Andrej Čilag
 */

#include "io.h"

#include "stm32f4xx.h"
#include <stdint.h>

static void
porta_init();

static uint8_t const LED = 5; // -> PA5

void
io_init()
{
  porta_init();
}

static void
porta_init()
{
  // Dozvola takta na port-u A
  //RCC->AHB1ENR = RCC->AHB1ENR | 1;
  RCC->AHB1ENR |= (0b1 << 0);

  // Podešavanje pina kao izlaz
  GPIOA->MODER &= ~(0b11 << 2 * LED);
  GPIOA->MODER |= (0b01 << 2 * LED);

  // Pin podešen kao push-pull
  GPIOA->OTYPER &= ~(0b1 << LED);

  // Maksimalna brzina pin-a
  GPIOA->OSPEEDR |= (0b11 << 2 * LED);

  // Bez pull-up/down-a
  GPIOA->PUPDR &= ~(0b11 << 2 * LED);
}

void
io_led (bool status)
{
  if (true == status)
    {
      GPIOA->ODR |= (0b1 << LED);
    }
  else
    {
      GPIOA->ODR &= ~(0b1 << LED);
    }
}
