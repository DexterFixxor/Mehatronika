/*
 * robot.h
 *
 *  Created on: Nov 13, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULI_ROBOT_ROBOT_H_
#define LIB_MODULI_ROBOT_ROBOT_H_

#include "Moduli/Odometrija/odom.h"
#include "Moduli/BDC/bdc.h"
#include "Moduli/PID/pid.h"

typedef struct
{
	sOdom_t* odom;

	sMotor_t* motor1;
	sMotor_t* motor2;

	sPID_t* pid_poz_lin;
	sPID_t* pid_poz_ang;

	sPID_t* pid_vel_lin;
	sPID_t* pid_vel_ang;

	float goalX, goalY, goalTheta;

}sRobot_t;

extern volatile sRobot_t* robot;

void Robot_Init();

void Robot_VelocityControlLoop(float v, float w);

void Robot_GoTo(float x, float y, float theta);
void Robot_PositionControlLoop();

#endif /* LIB_MODULI_ROBOT_ROBOT_H_ */
