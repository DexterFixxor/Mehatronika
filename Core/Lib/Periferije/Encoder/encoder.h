/*
 * encoder.h
 *
 *  Created on: Oct 26, 2023
 *      Author: dexter
 */

#ifndef LIB_PERIFERIJE_ENCODER_ENCODER_H_
#define LIB_PERIFERIJE_ENCODER_ENCODER_H_

#include <stm32f4xx.h>

#define ENC_DEFAULT_VALUE (0xFFFF >> 1)

void EncoderRight_Init();
int EncoderRight_GetDeltaInc();

void EncoderLeft_Init();
int EncoderLeft_GetDeltaInc();

void Encoders_Init();

#endif /* LIB_PERIFERIJE_ENCODER_ENCODER_H_ */
