/*
 * timer.c
 *
 *  Created on: Oct 19, 2023
 *      Author: dexter
 */

#include "timer.h"
#include "Modules/Odom/odom.h"
#include "Modules/Robot/robot.h"

volatile unsigned int system_ms = 0;

volatile sTimerFlags_t timer_flags;
volatile unsigned int timeout_ms = 0;

void TIMER_init()
{
	RCC->APB2ENR |= (0b1 << RCC_APB2ENR_TIM1EN_Pos);

	//Podesavanje preskalera i timer_period-a
	// APB2 max 84 MHz, posto smo mi stavili max frekvenciju, na toliko nam radi i APB1

	TIM1->PSC = 84 - 1;
	TIM1->ARR = 1000 - 1;

	// Dozvola prekida brojaca
	TIM1->DIER |= (0b1 << 0);
	TIM1->EGR |= (1 << 0);
	// Enable brojac
	TIM1->CR1 |= (0b1 << 0);
	TIM1->CR1 |= (1 << 2);

	while(! (TIM1->SR & (1 << 0)));

	NVIC->ISER[0] |= (1 << 25);
}

void TIMER_delay(unsigned int ms)
{
	unsigned int tmp_ms = system_ms;

	while ( (system_ms - tmp_ms) < ms);
}

void TIMER_timeout(unsigned int ms)
{
	if(timer_flags.flg_timeoutStart == 0)
			{
				timeout_ms = ms;
				timer_flags.flg_timeoutStart = 1;
				timer_flags.flg_timeoutEnd = 0;
			}
}

void TIM1_UP_TIM10_IRQHandler()
{
	if((TIM1->SR & (0b1 << 0)) == 0b1)
		{
			system_ms++;
			Odom_update(1);
			/*
			 * TODO:
			 * 	- update Odom_update(x), da se poziva na svaku ms
			 * 	- kreirati motor struct, gde ce se svaku ms azurirate w_r i w_l
			 * 	- implementacija ispod gde se Encoder1_GetDeltaInc() poziva u okviru
			 * 		spiska argumenata funkcije nije dobra, jer se CNT enkodera u okviru te funkcije
			 * 		restartuje pri svakom pozivu na pocetnu vrednost (ovo se isto desava u Odom_update)
			 * 		i to ce napraviti problem ZASIGURNO!!!
			 */

			// each 1ms, velocity control loop
			PID_ComputeOutput(pid_motor1, robot->w_r_ref, odom->w_r);
			HBridge_SetPWM(VOLATE_2_DUC(pid->output));
			//TODO: motor2

			if (system_ms % 10 == 0)
			{

				Robot_computeVelocities();
			}



			if(timer_flags.flg_timeoutStart == 1 && (timeout_ms > 0))
			{
				timeout_ms--;
			}
			if(timer_flags.flg_timeoutStart == 1 && timeout_ms == 0)
			{
				timer_flags.flg_timeoutEnd = 1;
				timer_flags.flg_timeoutStart = 0;
			}

			TIM1->SR &= ~(0b1 << 0);
		}
}
