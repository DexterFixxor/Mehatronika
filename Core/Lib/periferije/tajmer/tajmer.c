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

static void
tim3_init ()
{
  RCC->AHB1ENR |= (0b1 << 1); // Dozvola takta za GPIOB
  RCC->APB1ENR |= (0b1 << 1); // Dozvola takta za TIM3

  // Podešavanje pinova
  uint8_t const KANAL_A = 4;
  uint8_t const KANAL_B = 5;

  // Podešavanje pinova da rade kao alternativna funkcija
  GPIOB->MODER &= ~(0b11 << 2 * KANAL_A);
  GPIOB->MODER &= ~(0b11 << 2 * KANAL_B);

  GPIOB->MODER |= (0b10 << 2 * KANAL_A);
  GPIOB->MODER |= (0b10 << 2 * KANAL_B);

  // Odabir alternativne funkcije
  GPIOB->AFR[0] &= ~(0b1111 << 4 * KANAL_A);
  GPIOB->AFR[0] &= ~(0b1111 << 4 * KANAL_B);

  uint8_t const ALTERNATIVNA_FUNKCIJA = 2;
  GPIOB->AFR[0] |= (ALTERNATIVNA_FUNKCIJA << 4 * KANAL_A);
  GPIOB->AFR[0] |= (ALTERNATIVNA_FUNKCIJA << 4 * KANAL_B);

  // Podešavanje tajmera
  TIM3->PSC = 0; // Nećemo da smanjujemo rezoluciju
  TIM3->ARR = 0xFFFF; // Bitno je da najveći bit bude 1 zbog negativne brzine

  // Podešavanje kvadraturnog dekodiranja
  // Gledamo sve četiri promene signala
  TIM3->SMCR &= ~(0b111 << 0);
  TIM3->SMCR |= (0b011 << 0);

  // Povezujemo kanale enkodera tajmera, sa kanalom samog tajmera
  TIM3->CCMR1 &= ~(0b11 << 0);
  TIM3->CCMR1 |= (0b01 << 0);
  TIM3->CCMR1 &= ~(0b11 << 8);
  TIM3->CCMR1 |= (0b01 << 8);

  // Invertovanje kanala (odnosno neinvertovanje kanala)
  TIM3->CCER &= ~((0b1 << 1) | (0b1 << 3)); // neinvertovanje kanala 1
  TIM3->CCER &= ~((0b1 << 5) | (0b1 << 7)); // neinvertovanje kanala 2

  TIM3->CR1 |= (0b1 << 0); // Uključivanje tajmera
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
