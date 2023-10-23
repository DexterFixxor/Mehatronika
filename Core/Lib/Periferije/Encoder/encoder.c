/*
 * encoder.c
 *
 *  Created on: Oct 23, 2023
 *      Author: dexter
 */


#include "encoder.h"

void Encoder1_Init()
{
	/*
	 * Podesavanje GPIO port B, pinovi 6 i 7
	 */

	// Palimo clock signal na GPIOB
	RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOBEN_Pos);

	/* Podesimo GPIOB PB6 i PB7 kao Alt. funkcija */
	GPIOB->MODER &= ~(0b11 << 6 * 2 | 0b11 << 7 * 2);
	GPIOB->MODER |=  (0b10 << 6 * 2 | 0b10 << 7 * 2);

	// PB6->TIM4 CH1; PB7->TIM4 CH2
	GPIOB->AFR[0] &= ~(0b1111 << 6 * 4 | 0b1111 << 7 * 4);
	GPIOB->AFR[0] |=  (2 << 6 * 4 | 2 << 7 * 4);

	// Podesavanje tajmera

	RCC->APB1ENR |= (0b1 << RCC_APB1ENR_TIM4EN_Pos);

	TIM4->PSC = 0;
	TIM4->ARR = 0xFFFF - 1;

	// Encoder counts on both up and down edges of CH1 and CH2
	TIM4->SMCR &= ~(0b111 << 0 * 3);
	TIM4->SMCR |=  (0b011 << 0 * 3);

	TIM4->CCMR1 &= ~(0b11 << 0 | 0b11 << 8);
	TIM4->CCMR1 |=  (0b01 << 0 | 0b01 << 8);

	TIM4->CR1 |= (0b1 << 0); // CEN bit: enable counter
	TIM4->EGR |= (0b1 << 0);

	TIM4->CNT = ENC_DEFAULT_VALUE;
}

int Encoder1_GetDeltaInc()
{
	int cnt = TIM4->CNT - ENC_DEFAULT_VALUE;
	TIM4->CNT = ENC_DEFAULT_VALUE;

	return cnt;
}


void Encoder2_Init()
{
	/*
	 * -------------------------------
	 * CONFIGURE GPIO Port A pin 15 & port B pin 3
	 * -------------------------------
	 */

		// Enable Clock for GPIO
		RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOBEN_Pos | 0b1 << RCC_AHB1ENR_GPIOAEN_Pos);

		GPIOB->MODER &= ~(0b11 << 3 * 2);
		GPIOB->MODER |=  (0b10 << 3 * 2);

		GPIOA->MODER &= ~(0b11 << 15 * 2);
		GPIOA->MODER |=  (0b10 << 15 * 2);


		GPIOB->AFR[0] &= ~(0b1111 << 3 * 4);
		GPIOB->AFR[0] |=  (0b0001 << 3 * 4);

		GPIOA->AFR[1] &= ~(0b1111 << 7 * 4);
		GPIOA->AFR[1] |=  (0b0001 << 7 * 4);

		/*
		 * -----------------------------
		 * CONFIGURE TIM IN ENCODER MODE
		 * -----------------------------
		 */
		RCC->APB1ENR |= (0b1 << RCC_APB1ENR_TIM2EN_Pos);
		//Enable counter
		TIM2->CR1 |= (0b1 << 0);

		// Counts up/down on both edges (quadratic decoding)
		TIM2->SMCR &= ~(0b111 << 0);
		TIM2->SMCR |= (0b011 << 0);

		// Channel mapping A->TI1, B->TI2 (default)
		TIM2->CCMR1 &= ~(0b11 << 0 | 0b11 << 8); // clear/reset
		TIM2->CCMR1 |=  (0b01 << 0 | 0b01 << 8); // set as input

		// re-initialize the counter and generates an update of the registers
		TIM2->EGR |= (0b1 << 0);
		TIM2->CNT = ENC_DEFAULT_VALUE;

		TIM2->PSC = 0;
		TIM2->ARR = 0xFFFF - 1;

}

/*
 * Returns encoder increments change since last function call. Needs to be called often in order to avoid overflow/underflow.
 */
int Encoder2_GetDeltaInc()
{
	int ret = TIM2->CNT - ENC_DEFAULT_VALUE;
	TIM2->CNT = ENC_DEFAULT_VALUE;

	return ret;
}

void Encoders_Init()
{
	Encoder1_Init();
	Encoder2_Init();
}
