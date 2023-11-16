/*
 * odom.c
 *
 *  Created on: Oct 26, 2023
 *      Author: dexter
 */

#include "Periferije/Encoder/encoder.h"
#include "odom.h"
#include <stdlib.h>
#include <math.h>

volatile sOdom_t* odom;

void Odom_init(float x, float y, float theta)
{
	odom = malloc(sizeof *odom);
	odom->pose = malloc(sizeof(sPose_t));
	odom->vel = malloc(sizeof *odom->vel);

	odom->pose->x = x;
	odom->pose->y = y;
	odom->pose->theta = theta;

	odom->vel->v = 0;
	odom->vel->w = 0;
}

void Odom_update(unsigned int delta_ms)
{
	float dt_sec = delta_ms / 1000.;

	int enc_r = EncoderRight_GetDeltaInc();
	int enc_l = EncoderLeft_GetDeltaInc();

	// INC * RAD/INC * 1/s * m
	odom->w_r = enc_r * INC_2_RAD / dt_sec;
	odom->w_l = enc_l * INC_2_RAD / dt_sec;

	float v_r = odom->w_r * WHEEL_R;
	float v_l = odom->w_l * WHEEL_R;

	float v = (v_r + v_l)/2;
	float w = (v_r - v_l)/WHEEL_DISTANCE;

	odom->vel->v = v;
	odom->vel->w = w;

	odom->pose->x += v * cos(odom->pose->theta + (w*dt_sec/2)) * dt_sec;
	odom->pose->y += v * sin(odom->pose->theta + (w*dt_sec/2)) * dt_sec;
	odom->pose->theta += w * dt_sec;
}
