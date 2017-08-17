/*
 * DAC80004.c
 *
 *  Created on: 2017¦~8¤ë11¤é
 *      Author: hugoc_000
 */

#define	DAC80004_LD_PORT		GPIOB
#define	DAC80004_LD_PIN			GPIO_PIN_0
#define	DAC80004_CS_PORT		GPIOB
#define	DAC80004_CS_PIN			GPIO_PIN_1

#ifdef	SPI2_TIMEOUT
#undef	SPI2_TIMEOUT
#endif
#define	SPI2_TIMEOUT			(uint32_t)100


#include "DAC80004.h"
#include <string.h>

typedef union _tag_DAC80004_REG
{
    uint8_t     byte[4];
    uint32_t    dword;
    struct
    {
        uint32_t    CH_A        : 1;
        uint32_t    CH_B        : 1;
        uint32_t    CH_C        : 1;
        uint32_t    CH_D        : 1;
        uint32_t    DAC_Data    : 16;
        uint32_t    Channel     : 4;
        uint32_t    Command     : 4;
        uint32_t    RW          : 1;
        uint32_t                : 3;
    };
    struct
    {
        uint32_t    CM0         : 1;
        uint32_t    CM1         : 1;
        uint32_t                : 2;
        uint32_t    DRDY        : 1;
        uint32_t                : 3;
        uint32_t    PD0         : 1;
        uint32_t    PD1         : 1;
        uint32_t                : 22;
    };
}DAC80004_REG;

typedef struct _tagDAC80004_State
{
    DAC80004_REG RegA;
    DAC80004_REG RegB;
    DAC80004_REG RegC;
    DAC80004_REG RegD;
    uint8_t DataA[4];
    uint8_t DataB[4];
    uint8_t DataC[4];
    uint8_t DataD[4];
}DAC80004_State;

DAC80004_State DAC80004;

void DAC80004_SetupData(DAC80004_REG * pReg, uint8_t * pByte, uint16_t Data) {
    pReg->DAC_Data = Data;
    pByte[0] = pReg->byte[3];
    pByte[1] = pReg->byte[2];
    pByte[2] = pReg->byte[1];
    pByte[3] = pReg->byte[0];
}


int DAC80004_WriteReg(uint8_t* data, _Bool ld) {

	HAL_GPIO_WritePin(DAC80004_CS_PORT, DAC80004_CS_PIN, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&hspi2, data, 4, SPI2_TIMEOUT)!=HAL_OK) return 1;
	HAL_GPIO_WritePin(DAC80004_CS_PORT, DAC80004_CS_PIN, GPIO_PIN_SET);
    if(ld) {
    	HAL_GPIO_WritePin(DAC80004_LD_PORT, DAC80004_LD_PIN, GPIO_PIN_RESET);
        HAL_Delay(1);
    	HAL_GPIO_WritePin(DAC80004_LD_PORT, DAC80004_LD_PIN, GPIO_PIN_SET);
    }
    return 0;
}


int DAC80004_Set(uint16_t A, uint16_t B, uint16_t C, uint16_t D, _Bool load) {
    DAC80004_SetupData(&DAC80004.RegA, DAC80004.DataA, A);
    DAC80004_SetupData(&DAC80004.RegB, DAC80004.DataB, B);
    DAC80004_SetupData(&DAC80004.RegC, DAC80004.DataC, C);
    DAC80004_SetupData(&DAC80004.RegD, DAC80004.DataD, D);
    if (DAC80004_WriteReg(DAC80004.DataA,0)) return 1;
    if (DAC80004_WriteReg(DAC80004.DataB,0)) return 1;
    if (DAC80004_WriteReg(DAC80004.DataC,0)) return 1;
    if (DAC80004_WriteReg(DAC80004.DataD,load)) return 1;
    return 0;
}


void DAC80004_Init(void) {
	HAL_GPIO_WritePin(DAC80004_CS_PORT, DAC80004_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DAC80004_LD_PORT, DAC80004_LD_PIN, GPIO_PIN_SET);
    memset(&DAC80004, 0, sizeof(DAC80004));
    DAC80004.RegB.Channel = 1;
    DAC80004.RegC.Channel = 2;
    DAC80004.RegD.Channel = 3;
}

