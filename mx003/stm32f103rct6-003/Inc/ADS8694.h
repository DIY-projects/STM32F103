/*
 * ADS8694.h
 *
 *  Created on: 2017¦~8¤ë12¤é
 *      Author: hugoc_000
 */

#ifndef ADS8694_H_
#define ADS8694_H_


#include "stm32f1xx_hal.h"
#include <stdint.h>

extern SPI_HandleTypeDef hspi2;

void ADS8694_Init(void);
uint32_t ADS8694_Read(int);

#endif /* ADS8694_H_ */
