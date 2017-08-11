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

#define SPI2_TIMEOUT			100


#include "DAC80004.h"
#include "stm32f1xx_hal.h"
#include <string.h>


//#include "Chip.h"
//#include "Compiler.h"
//#include "ConfigPins.h"
//#include "ConfigDevices.h"
//
//#include "Board.h"
//#include "Oscillator.h"
//#include "MSPI.h"

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
extern SPI_HandleTypeDef hspi2;

void DAC80004_SetupData(DAC80004_REG * pReg, uint8_t * pByte, uint16_t Data)
{
    pReg->DAC_Data = Data;
    pByte[0] = pReg->byte[3];
    pByte[1] = pReg->byte[2];
    pByte[2] = pReg->byte[1];
    pByte[3] = pReg->byte[0];
}

//void DAC80004_Write_Init(void *pObj)
//{
//    LAT(PIN_DAC80004_CS) = 0;
//}
//
//void DAC80004_Write_Done(void * pObj)
//{
//    bool load = (bool)pObj;
//    LAT(PIN_DAC80004_CS) = 1;
//    if(load)
//    {
//        LAT(PIN_DAC80004_LD) = 0;
//        DelayUs(20);
//        LAT(PIN_DAC80004_LD) = 1;
//    }
//}

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


int DAC80004_Set(uint16_t A, uint16_t B, uint16_t C, uint16_t D, _Bool load)
{
    DAC80004_SetupData(&DAC80004.RegA, DAC80004.DataA, A);
    DAC80004_SetupData(&DAC80004.RegB, DAC80004.DataB, B);
    DAC80004_SetupData(&DAC80004.RegC, DAC80004.DataC, C);
    DAC80004_SetupData(&DAC80004.RegD, DAC80004.DataD, D);
//    MSPIEnqueue(DAC80004_SPI_CHANNEL, 0, DAC80004.DataA, 0, 4, 0, DAC80004_Write_Init, DAC80004_Write_Done);
//    MSPIEnqueue(DAC80004_SPI_CHANNEL, 0, DAC80004.DataB, 0, 4, 0, DAC80004_Write_Init, DAC80004_Write_Done);
//    MSPIEnqueue(DAC80004_SPI_CHANNEL, 0, DAC80004.DataC, 0, 4, 0, DAC80004_Write_Init, DAC80004_Write_Done);
//    MSPIEnqueue(DAC80004_SPI_CHANNEL, 0, DAC80004.DataD, 0, 4, (void *)load, DAC80004_Write_Init, DAC80004_Write_Done);
    if (DAC80004_WriteReg(DAC80004.DataA,0)) return 1;
    if (DAC80004_WriteReg(DAC80004.DataB,0)) return 1;
    if (DAC80004_WriteReg(DAC80004.DataC,0)) return 1;
    if (DAC80004_WriteReg(DAC80004.DataD,load)) return 1;
    return 0;
}

void DAC80004_Init(void)
{
	__HAL_SPI_ENABLE(&hspi2);

	HAL_GPIO_WritePin(DAC80004_CS_PORT, DAC80004_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DAC80004_LD_PORT, DAC80004_LD_PIN, GPIO_PIN_SET);
//	LAT(PIN_DAC80004_CS) = 1;
//    LAT(PIN_DAC80004_LD) = 1;
//    TRIS(PIN_DAC80004_CS) = 0;
//    TRIS(PIN_DAC80004_LD) = 0;

    memset(&DAC80004, 0, sizeof(DAC80004));
    DAC80004.RegB.Channel = 1;
    DAC80004.RegC.Channel = 2;
    DAC80004.RegD.Channel = 3;
}

