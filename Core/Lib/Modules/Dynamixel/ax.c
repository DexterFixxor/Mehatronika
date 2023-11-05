/*
 * ax.c
 *
 *  Created on: Nov 2, 2023
 *      Author: dexter
 */

#include "ax.h"
#include "Periferije/UART/uart.h"

void AX_SendPacket(
		unsigned char id,
		unsigned char instruction,
		unsigned char* params,
		unsigned int n_params)

{
	/* 0xFF 	0xFF 	Packet ID 	Length 	Instruction 	Param 1 	… 	Param N 	CHKSUM */
	unsigned char length = n_params + 2;

	unsigned int checksum = id + length + instruction;

	for (int i = 0; i < n_params; i++)
		checksum += *(params + i);

	UART1_SendChar(0xFF);
	UART1_SendChar(0xFF);
	UART1_SendChar(id);
	UART1_SendChar(length);
	UART1_SendChar(instruction);

	UART1_SendArray(params, n_params);

	checksum = ~(checksum & 0xFF);
	UART1_SendChar((unsigned char)checksum);

}

void AX_SetLED(unsigned char id, unsigned char state)
{
	unsigned char params[] = {25, state};

	AX_SendPacket(id, AX_WRITE, params, 2);
}

void AX_Move(unsigned char id, float angle)
{
	if (angle > 300)
		angle = 300;
	if (angle < 0)
		angle = 0;
	unsigned int angle_in_inc = (unsigned int) ((angle * 1024/300.0) - 1);
	unsigned char param1 = (angle_in_inc >> 8) & 0xFF;
	unsigned char param2 = (angle_in_inc & 0xFF);


	unsigned char params[] = {30, param1, param2};

	AX_SendPacket(id, AX_WRITE, params, 3);
}
