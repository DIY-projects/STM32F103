/*
 * DAC80004.h
 *
 *  Created on: 2017¦~8¤ë11¤é
 *      Author: hugoc_000
 */

#ifndef DAC80004_H_
#define DAC80004_H_


#include <stdbool.h>
#include <stdint.h>

void DAC80004_Init(void);
int DAC80004_Set(uint16_t, uint16_t, uint16_t, uint16_t, _Bool);

#endif /* DAC80004_H_ */
