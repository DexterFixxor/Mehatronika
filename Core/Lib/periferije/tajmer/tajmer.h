/*
 * tajmer.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Andrej Čilag
 */

#ifndef LIB_PERIFERIJE_TAJMER_TAJMER_H_
#define LIB_PERIFERIJE_TAJMER_TAJMER_H_

#include <stdint.h>

void
tajmer_init();
void
tajmer_delay (uint32_t ms);

#endif /* LIB_PERIFERIJE_TAJMER_TAJMER_H_ */
