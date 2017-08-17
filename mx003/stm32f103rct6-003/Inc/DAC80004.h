/*
 * DAC80004.h
 *
 *  Created on: 2017¦~8¤ë11¤é
 *      Author: hugoc_000
 */

#ifndef DAC80004_H_
#define DAC80004_H_

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

extern SPI_HandleTypeDef hspi2;

void DAC80004_Init(void);
int DAC80004_Set(uint16_t, uint16_t, uint16_t, uint16_t, _Bool);

#endif /* DAC80004_H_ */
