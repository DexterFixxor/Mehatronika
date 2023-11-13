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

	pid->prevMeasured = 0;

}

void PID_ComputeOutput(sPID_t* pid, float ref, float measured)
{
	pid->error[0] = ref - measured;


	pid->out +=
			pid->Kp * (measured - pid->prevMeasured) +
			pid->Ki * pid->error[0] +
			pid->Kd * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);

	if(pid->out > pid->outMax)
		pid->out = pid->outMax;
	else if(pid->out < pid->outMin)
		pid->out = pid->outMin;

	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	pid->prevMeasured = measured;
}
