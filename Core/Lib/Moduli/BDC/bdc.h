/*
 * bdc.h
 *
 *  Created on: Nov 13, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULI_BDC_BDC_H_
#define LIB_MODULI_BDC_BDC_H_

#include "Moduli/PID/pid.h"

typedef struct
{
	sPID_t* pid;
	float maxVelocity;
	float maxAcc;

	float refVelocity;

}sMotor_t;

extern volatile sMotor_t* motor1;
extern volatile sMotor_t* motor2;

void Motor_Init();

/*
 * refx [-maxVelocity, maxVelocity]
 */
void Motor_SetRefVel(float ref1, float ref2);

int Motor1_GetDeltaInc();
int Motor2_GetDeltaInc();

void Motor_controlLoop();

#endif /* LIB_MODULI_BDC_BDC_H_ */
