/*
 * robot.h
 *
 *  Created on: Nov 16, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULES_ROBOT_ROBOT_H_
#define LIB_MODULES_ROBOT_ROBOT_H_

typedef struct
{
	float x_ref, y_ref, theta_ref;
	float v_ref, w_ref;
	float w_r_ref, w_l_ref;
	unsigned char phase;

	float Kp_lin, Kp_ang;

}sRobot_t;

volatile sRobot_t* robot;

void Robot_init(float Kp_lin, float Kp_ang);

// SET ref values in Robot struct
void Robot_goto(float x, float y, float theta, unsigned char override_phase);

// Update linear and angular velocities of robot's base
void Robot_computeVelocities();

#endif /* LIB_MODULES_ROBOT_ROBOT_H_ */
