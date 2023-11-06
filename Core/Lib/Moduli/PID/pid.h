/*
 * pid.h
 *
 *  Created on: Nov 6, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULI_PID_PID_H_
#define LIB_MODULI_PID_PID_H_

typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float error[3]; // e[0] = e[k], e[1] = e[k-1], e[2] = e[k-2]

	float outMin;
	float outMax;
	float out;
}sPID_t;

volatile sPID_t *pid;

void PID_Init(sPID_t* pid, float Kp, float Ki, float Kd, float outMin, float outMax);
void PID_ComputeOutput(sPID_t* pid, float ref, float measured, float prev_measured);

#endif /* LIB_MODULI_PID_PID_H_ */
