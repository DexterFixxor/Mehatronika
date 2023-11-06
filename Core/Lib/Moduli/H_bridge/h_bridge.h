/*
 * h_bridge.h
 *
 *  Created on: Nov 6, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULI_H_BRIDGE_H_BRIDGE_H_
#define LIB_MODULI_H_BRIDGE_H_BRIDGE_H_

typedef enum
{
	STOP,
	CW,
	CCW
}eMotorDir_t;


void Motor1_HBridge_Init();
void Motor1_Enable();
void Motor1_Disable();
void Motor1_Direction(eMotorDir_t dir);
/*
 * duc: Duty Cycle in range [-1,1]
 */
void Motor1_SetDuC(float duc);

#endif /* LIB_MODULI_H_BRIDGE_H_BRIDGE_H_ */
