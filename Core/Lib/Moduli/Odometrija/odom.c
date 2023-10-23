/*
 * odom.c
 *
 *  Created on: Oct 23, 2023
 *      Author: dexter
 */
#include <stdlib.h>
#include <math.h>

#include "Periferije/Encoder/encoder.h"
#include "odom.h"

volatile sOdom_t* odom;

void Odom_init(float x, float y, float theta)
{
	odom = (sOdom_t*)malloc(sizeof(sOdom_t));
	odom->pose = (sPose_t*)malloc(sizeof(sPose_t));
	odom->vel = (sVelocity_t*)malloc(sizeof(sVelocity_t));

	odom->pose->x = x;
	odom->pose->y = y;
	odom->pose->theta = theta;

	odom->vel->angular = 0;
	odom->vel->linear = 0;
}


void Odom_update(unsigned int dt)
{
	/*
	 * dt: parametar u milisekundama
	 */
	int enc_desni = Encoder1_GetDeltaInc(); // pomeraj u inkrementima
	int enc_levi = Encoder2_GetDeltaInc();

	float v_r = enc_desni * INC_2_RAD * WHEEL_RADIUS; 	// [m/s]
	float v_d = enc_levi * INC_2_RAD * WHEEL_RADIUS;		// [m/s]

	float v = (v_r + v_d)/2;
	float w = (v_r - v_d)/WHEEL_DISTANCE;
	// omega [rad/sec]

	/*
	 * Dodaje se w/2 u cos i sin da bi se povecala preciznost (stabilonst racunanja)
	 * Po kinematskom modelu diferencijalnog robota je moguce i
	 * (theta[k] + theta[k+1])/2, odnosno srednja vrednost
	 */
	odom->pose->x += v * cos(odom->pose->theta + w/2);
	odom->pose->y += v * sin(odom->pose->theta + w/2);
	odom->pose->theta += w; // theta[k+1] = theta[k] + w

	odom->vel->linear = v;
	odom->vel->angular = w;
}
