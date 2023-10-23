/*
 * encoder.h
 *
 *  Created on: Oct 23, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_ENCODER_ENCODER_H_
#define LIB_PERIFERIJE_ENCODER_ENCODER_H_

#include <stm32f4xx.h>

#define ENC_DEFAULT_VALUE (0xFFFF >> 1)

void Encoder1_Init();
int Encoder1_GetDeltaInc();
void Encoder2_Init();
int Encoder2_GetDeltaInc();

void Encoders_Init();

#endif /* LIB_PERIFERIJE_ENCODER_ENCODER_H_ */
