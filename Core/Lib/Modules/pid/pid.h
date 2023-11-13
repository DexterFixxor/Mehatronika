/*
 * pid.h
 *
 *  Created on: Nov 9, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULES_PID_PID_H_
#define LIB_MODULES_PID_PID_H_

typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float output;
	float outMax;
	float outMin;

	float error[3]; // error[0] = e[T], error[1] = e[T-1], error[2]=e[T-2]

	float prev_measure;
}sPID_t;

volatile extern sPID_t *pid;

void PID_Init(sPID_t* pid, float Kp, float Kd, float Ki, float outMax, float outMin);
void PID_ComputeOutput(sPID_t* pid, float ref, float measure);


#endif /* LIB_MODULES_PID_PID_H_ */
