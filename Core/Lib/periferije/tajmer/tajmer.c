/*
 * tajmer.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Andrej Čilag
 */

#include "tajmer.h"

#include "stm32f4xx.h"
#include <stdbool.h>

static void
tim2_init ();

volatile uint32_t sistemsko_vreme = 0;
bool flag_delay = true;

void
tajmer_init()
{
  tim2_init();
}

void
tajmer_delay (uint32_t ms)
{
  uint32_t pocetno_vreme1 = sistemsko_vreme;
  while(sistemsko_vreme <= pocetno_vreme1 + ms)
    {
      __NOP();
    }
}

bool
tajmer_delay_nb (uint32_t ms)
{
  static uint32_t pocetno_vreme;

  if (flag_delay == true)
    {
      pocetno_vreme = sistemsko_vreme;
      flag_delay = false;
    }

  if(sistemsko_vreme <= pocetno_vreme + ms)
    {
      return false;
    }
  else
    {
      flag_delay = true;
      return true;
    }
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

  uint8_t const TIM2_PREKID = 28;
  NVIC->ISER[0] |= (0b1 << TIM2_PREKID);

  TIM2->CR1 |= (0b1 << 0); // Uključivanje tajmera
}

void
TIM2_IRQHandler ()
{
  // Prekidna rutina koja se poziva na svaku ms
  if ((TIM2->SR & (0b1 << 0)) == (0b1 << 0))
    {
      // Sami čistimo bit, da bi sledeći put mogli da detektujemo prekid
      TIM2->SR &= ~(0b1 << 0);

      sistemsko_vreme ++;
    }

}
