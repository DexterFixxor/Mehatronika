/*
 * ax.h
 *
 *  Created on: Nov 2, 2023
 *      Author: dexter
 */

#ifndef LIB_MODULES_DYNAMIXEL_AX_H_
#define LIB_MODULES_DYNAMIXEL_AX_H_

/* AX instructions */
#define AX_PING		0x01
#define AX_READ  	0x02
#define AX_WRITE 	0x03



void AX_SendPacket(
		unsigned char id,
		unsigned char instruction,
		unsigned char* params,
		unsigned int n_params);

void AX_SetLED(unsigned char id, unsigned char state);
void AX_Move(unsigned char id, float angle);

#endif /* LIB_MODULES_DYNAMIXEL_AX_H_ */
