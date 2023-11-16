/*
 * pid.c
 *
 *  Created on: Nov 9, 2023
 *      Author: dexter
 */

#include "pid.h"

volatile sPID_t *pid_motor1;

void PID_Init(sPID_t* pid, float Kp, float Kd, float Ki, float outMax, float outMin)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;

	pid->output = 0;
	pid->outMax = outMax;
	pid->outMin = outMin;

	pid->prev_measure = 0;
}

void PID_ComputeOutput(sPID_t* pid, float ref, float measure)
{
	pid->error[0] = ref - measure;
	pid->output +=
			pid->Kp * (measure - pid->prev_measure) +
			pid->Ki * pid->error[0] +
			pid->Kd * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);

	if (pid->output > pid->outMax)
		pid->output = pid->outMax;
	else if(pid->output < pid->outMin)
		pid->output = pid->outMin;

	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	pid->prev_measure = measure;
}
