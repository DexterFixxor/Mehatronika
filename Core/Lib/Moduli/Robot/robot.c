/*
 * robot.c
 *
 *  Created on: Nov 13, 2023
 *      Author: dexter
 */

#include "robot.h"
#include <math.h>

volatile sRobot_t* robot;

static unsigned char phase = 0;

void Robot_Init()
{
	robot->motor1 = motor1;
	robot->motor2 = motor2;

	robot->odom = odom;

	PID_Init(robot->pid_poz_lin, 1, 1, 1, -1, 1);
	PID_Init(robot->pid_poz_ang, 1, 1, 1, -1, 1);
	PID_Init(robot->pid_vel_lin, 1, 1, 1, -1, 1);
	PID_Init(robot->pid_vel_ang, 1, 1, 1, -1, 1);

}

void Robot_GoTo(float x, float y, float theta)
{
	phase = 0;
	robot->goalX = x;
	robot->goalY = y;
	robot->goalTheta = theta;
}

void Robot_PositionControlLoop()
{
	float v	= 0;
	float w = 0;
	/*
	 * 3 faze, rotacija->translacija->rotacija
	 */

	switch(phase)
	{

	case 0://rotaciju
		float phi = atan2(robot->goalY - robot->odom->pose->y, robot->goalX - robot->odom->pose->x);
		//trenutna orijentacija ka targetu
		float phi_error = phi - robot->odom->pose->theta;

		// normalizacija ugla u rasponu od [-pi, pi]
		if (phi_error > PI)
			phi_error -= 2*PI;
		else if(phi_error < - PI)
			phi_error += 2*PI;

		w = robot->pid_poz_ang->Kp * phi_error;

		if ((phi_error > -0.05 && phi_error < 0.05) && (robot->odom->w < 0.01 && robot->odom->w > -0.01))
		{
			w = 0;
			phase = 1;
		}

		break;
	case 1://translacija
		float dist_error = sqrtf(
				pow(robot->goalY - robot->odom->pose->y,2) +
				pow(robot->goalX - robot->odom->pose->x,2)
		);

		unsigned char direction = 1;

		if (dist_error < 0.01)
		{
			float phi = atan2(robot->goalY - robot->odom->pose->y, robot->goalX - robot->odom->pose->x);
			if (!(((phi - robot->odom->theta) > -0.05) && ((phi - robot->odom->theta) < 0.05)))
				direction = -1;
		}

		v = robot->pid_poz_lin->Kp * dist_error * direction;

		if (dist_error < 0.01 && robot->odom->v < 0.1)
		{
			v = 0;
			phase = 2;
		}
		break;
	case 2: // rotacija

		float theta_error = robot->goalTheta - robot->odom->pose->theta;
		if (theta_error > PI)
			theta_error -= 2*PI;
		else if(theta_error < - PI)
			theta_error += 2*PI;

		w = robot->pid_poz_ang->Kp * theta_error;

		if ((theta_error > -0.05 && theta_error < 0.05) && (robot->odom->w < 0.01 && robot->odom->w > -0.01))
		{
			w = 0;
			phase = 3;
		}
		break;
	}

	float L = 0.25;
	float R = 0.07;
	Motor_SetRefVel(v + L * w/(2.0 * R), v - L * w/(2.0 * R));


}
