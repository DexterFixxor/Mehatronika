/*
 * odom.h
 *
 *  Created on: Oct 26, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULES_ODOM_ODOM_H_
#define LIB_MODULES_ODOM_ODOM_H_

#define PI				3.141592653589793238462643383279502884197
#define ENC_MAX_INC		(2048 * 4)
#define INC_2_RAD		(2*PI/ENC_MAX_INC)

#define WHEEL_DISTANCE	0.259 // [m]
#define WHEEL_D			0.071 // [m]
#define WHEEL_R			(WHEEL_D/2)
/*
 * x, y, Theta: pozicija x i y, i orijentacije u koordinatnom sistemu Odometrije
 * v, w : linearna brzina i ugaona brzina
 */

typedef struct
{
	float x;
	float y;
	float theta;
}sPose_t;

typedef struct
{
	float v;
	float w;
}sVelocity_t;

typedef struct
{
	sPose_t* pose;
	sVelocity_t* vel;

	float w_r;
	float w_l;
}sOdom_t;

extern volatile sOdom_t* odom;

void Odom_init(float x, float y, float theta);
void Odom_update(unsigned int delta_ms);

#endif /* LIB_MODULES_ODOM_ODOM_H_ */
