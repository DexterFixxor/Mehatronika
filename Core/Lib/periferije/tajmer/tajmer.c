/*
 * tajmer.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Andrej Čilag
 */

#include "tajmer.h"

#include "stm32f4xx.h"

static void
tim2_init ();

void
tajmer_init()
{
  tim2_init();
}

static void
tim2_init ()
{
  RCC->APB1ENR |= (0b1 << 0);

  // Podešavanje takta tajmera
  // 84MHZ -> 1kHZ
  // 1) 84MHz -> 1MHz
  TIM2->PSC = 84 - 1;
  // 2) 1MHz -> 1kHz
  TIM2->ARR = 1000 - 1;

  //TIM2->CR1 &= ~((0b1 << 1) | (0b1 << 2));
  TIM2->CR1 &= ~(0b1 << 1); // Dozvola događaja
  TIM2->CR1 &= ~(0b1 << 2); // Šta generiše događaj

  TIM2->EGR |= (0b1 << 0); // Reinicijalizacija tajmera

  TIM2->DIER |= (0b1 << 0); // Dozvola prekida

  while (!(TIM2->SR & (0b1 << 0)))
    {
      __NOP();
    }
  TIM2->SR &= ~(0b1 << 0);

  TIM2->CR1 |= (0b1 << 2);

  // TODO: Odabir prekidne rutine

  TIM2->CR1 |= (0b1 << 0); // Uključivanje tajmera
}
