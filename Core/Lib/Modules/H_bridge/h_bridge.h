/*
 * h_bridge.h
 *
 *  Created on: Nov 9, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULES_H_BRIDGE_H_BRIDGE_H_
#define LIB_MODULES_H_BRIDGE_H_BRIDGE_H_

typedef enum
{
	STOP,
	CW,
	CCW
}eDirection_t;

void HBridge_Init();
void HBridge_Enable();
void HBridge_SetDir(const eDirection_t dir);

#endif /* LIB_MODULES_H_BRIDGE_H_BRIDGE_H_ */
