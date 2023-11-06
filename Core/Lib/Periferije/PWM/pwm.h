/*
 * pwm.h
 *
 *  Created on: Nov 6, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_PWM_PWM_H_
#define LIB_PERIFERIJE_PWM_PWM_H_

#define MOTOR_ARR 1050 - 1

void Motor1_PWM_Init();
void Motor1_SetPWM(unsigned int duc_inc);

#endif /* LIB_PERIFERIJE_PWM_PWM_H_ */
