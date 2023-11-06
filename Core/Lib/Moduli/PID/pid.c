/*
 * pid.c
 *
 *  Created on: Nov 6, 2023
 *      Author: dexter
 */

#include "pid.h"
#include <stdlib.h>

volatile sPID_t* pid;

void PID_Init(sPID_t* pid, float Kp, float Ki, float Kd, float outMin, float outMax)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;

	pid->out = 0;
	pid->outMin = outMin;
	pid->outMax = outMax;

}

void PID_ComputeOutput(sPID_t* pid, float ref, float measured, float prev_measured)
{
	pid->error[0] = ref - measured;
	pid->output +=
			pid->Kp * (measured - prev_measured) +
			pid->Ki * pid->error[0] +
			pid->Kd * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);

	if(pid->output > pid->outMax)
		pid->output = pid->outMax;
	else if(pid->output < pid->outMin)
		pid->output = pid->outMin;

	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];
}
