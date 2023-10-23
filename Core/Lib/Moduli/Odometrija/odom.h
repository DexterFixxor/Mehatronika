/*
 * odom.h
 *
 *  Created on: Oct 23, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULI_ODOMETRIJA_ODOM_H_
#define LIB_MODULI_ODOMETRIJA_ODOM_H_

#define PI 				3.141592653589793238462643383279502884197

#define WHEEL_DISTANCE 	0.2595 // [m]
#define WHEEL_DIAMETER 	0.072 // [m]
#define WHEEL_RADIUS	(WHEEL_DIAMETER / 2)
#define WHEEL_O			(WHEEL_DIAMETER * PI)

#define ENC_NUM_INC		(2048 * 4)
#define INC_2_RAD		(2 * PI / ENC_NUM_INC) 	// [rad/inc]
#define INC_2_MM		(WHEEL_O/ENC_NUM_INC)	// [ mm/inc]
#define	MM_2_INC		(1 / INC_2_MM)

typedef struct
{
	float x;
	float y;
	float theta;
}sPose_t;

typedef struct
{
	float linear;
	float angular;
}sVelocity_t;

typedef struct
{
	sPose_t* pose;
	sVelocity_t* vel;
}sOdom_t;

extern volatile sOdom_t* odom;

void Odom_init(float x, float y, float theta);
void Odom_update(unsigned int dt);

#endif /* LIB_MODULI_ODOMETRIJA_ODOM_H_ */
