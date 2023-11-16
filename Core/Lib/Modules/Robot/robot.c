/*
 * robot.c
 *
 *  Created on: Nov 16, 2023
 *      Author: dexter
 */

#include "robot.h"
#include "Modules/Odom/odom.h"
#include <math.h>

volatile sRobot_t* robot;
volatile sOdom_t* odom;

static float normalize_angle_rad(float rad)
{
	if (rad > PI)
		return rad -= 2*PI;
	if (rad < -PI)
		return rad += 2*PI;
}

void Robot_init(float Kp_lin, float Kp_ang)
{
	robot->Kp_lin = Kp_lin;
	robot->Kp_ang = Kp_ang;

	// TODO: izmeniti u pocetne koordinate robota na stolu
	robot->x_ref = 0;
	robot->y_ref = 0;
	robot->theta_ref = 0;
}

void Robot_goto(float x, float y, float theta, unsigned char override_phase)
{
	robot->x_ref = x;
	robot->y_ref = y;
	robot->theta_ref = theta;

	if (robot->phase == 3 || override_phase != 0)
			robot->phase = 0;
}

void Robot_computeVelocities()
{
	float v = 0;
	float w = 0;

	float theta_eps = 0.01; // rad
	float dist_eps = 0.5;
	float gamma;
	float phi;
	float dist;

	// brzine levog i desnog tocka
	float w_r = 0;
	float w_l = 0;

	gamma = atan2(robot->y_ref - odom->pose->y, robot->x_ref - odom->pose->x);
	phi = normalize_angle_rad(gamma - odom->pose->theta); // angle error

	switch(robot->phase)
	{
	case 0: // Rot(z, phi)
		v = 0;
		w = robot->Kp_ang * phi;
		if ((phi < theta_eps && phi > -theta_eps) && (odom->vel->w > -theta_eps/10. && odom->vel->w < theta_eps/10.))
			robot->phase = 1;
		break;
	case 1: // Trans(x, dist)
		dist = sqrt(pow(robot->x_ref - odom->pose->x, 2) + pow(robot->y_ref - odom->pose->y, 2));
		if (phi > theta_eps || phi < - theta_eps)
			dist *= -1;

		v = robot->Kp_lin * dist;
		w = 0;

		if ((dist > -dist_eps && dist < dist_eps)
				&& (odom->vel->v > -dist_eps/10. && odom->vel->v < dist_eps/10.))
			robot->phase = 2;
		break;
	case 2: // Rot(z, phi')
		phi = normalize_angle_rad(robot->theta_ref - odom->pose->theta); // theta_error
		w = robot->Kp_ang * phi;
		v = 0;
		// OPCIONO: dodati proveru da li je dist poraslo, ako jeste vratiti se na phase=1
		if ((phi < theta_eps && phi > -theta_eps)
				&& (odom->vel->w > -theta_eps/10. && odom->vel->w < theta_eps/10.))
					robot->phase = 3;

		break;
	}

	robot->v_ref = v;
	robot->w_ref = w;

	float L = 0.25;
	float R = 0.07; //TODO: read from robot configuration
	float dt_sec = 0.001;
	robot->w_r_ref = v + w * L/(2*R);
	robot->w_l_ref = v - w * L/(2*R);

}
