/*
 * bdc.c
 *
 *  Created on: Nov 13, 2023
 *      Author: dexter
 */

#include "bdc.h"
#include "Moduli/H_bridge/h_bridge.h"

volatile sMotor_t* motor1;
volatile sMotor_t* motor2;


void Motor_Init()
{
	motor1->maxVelocity = 6; 	// rad/s
	motor1->maxAcc = 0.6;		// rad/s/s
	motor1->refVelocity = 0;
	PID_Init(motor1->pid, 1, 0.001, 0.1, -motor1->maxVelocity, motor1->maxVelocity);

	motor2->maxVelocity = 6;
	motor2->maxAcc = 0.6;
	motor2->refVelocity = 0;
	PID_Init(motor2->pid, 1, 0.001, 0.1, -motor2->maxVelocity, motor2->maxVelocity);
}

void Motor_SetRefVel(float ref1, float ref2)
{
	motor1->refVelocity = ref1;
	motor2->refVelocity = ref2;
}

void Motor_controlLoop()
{
	/// OPCIONO, dodati argument za protklo vreme (u ms) ukoliko je potrebno
	int delta_1_inc = Motor1_GetDeltaInc();
	int delta_2_inc = Motor2_GetDeltaInc();

	float delta_1 = delta_1_inc * 1215135151; // const je random broj trenutno
	float delta_2 = delta_2_inc * 12312515124; // opet random broj, PROMENITI u konverziju inc u rad

	float error1 = motor1->refVelocity - delta_1;
	float error2 = motor2->refVelocity - delta_2;

	PID_ComputeOutput(motor1->pid, motor1->refVelocity, delta_1);
	PID_ComputeOutput(motor2->pid, motor2->refVelocity, delta_2);

	Motor1_SetDuC(motor1->pid->out);
	//TODO: implement Motor2_SetDuC()

}
