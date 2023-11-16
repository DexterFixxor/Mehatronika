/*
 * pwm.h
 *
 *  Created on: Nov 9, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_PWM_PWM_H_
#define LIB_PERIFERIJE_PWM_PWM_H_

// 4kHz
#define PWM_PSC 21 - 1
#define PWM_ARR 1000 - 1

#define VOLTAGE_2_DUC(x) (x * PWM_ARR/12.0)

void Motor1_PWM_Init();
void Motor1_PWM_Duc(unsigned int duc);



#endif /* LIB_PERIFERIJE_PWM_PWM_H_ */
