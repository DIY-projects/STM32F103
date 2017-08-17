/*
 * ADS8694.c
 *
 *  Created on: 2017¦~8¤ë12¤é
 *      Author: hugoc_000
 */

#include "ADS8694.h"
#include "stm32f1xx_hal.h"

#include <stdint.h>

#ifdef	SPI2_TIMEOUT
#undef	SPI2_TIMEOUT
#endif
#define	SPI2_TIMEOUT		(uint32_t)100


#define ADS8694_RST_PORT		GPIOC
#define ADS8694_RST_PIN			GPIO_PIN_6
#define ADS8694_CS_PORT			GPIOC
#define ADS8694_CS_PIN			GPIO_PIN_7


// ADS8694 PROGRAM REGISTER MAP
// Auto Scan Sequencing Control
#define ADS8694_REG_ADDRESS_AUTO_SEQ_EN						0x01
#define ADS8694_REG_ADDRESS_CHANNEL_POWER_DOWN				0x02
// Device Features Selection Control
#define ADS8694_REG_ADDRESS_FEATURE_SELECT					0x03
// Range Select Registers
#define ADS8694_REG_ADDRESS_CHANNEL_0_INPUT_RANGE			0x05
#define ADS8694_REG_ADDRESS_CHANNEL_1_INPUT_RANGE			0x06
#define ADS8694_REG_ADDRESS_CHANNEL_2_INPUT_RANGE			0x07
#define ADS8694_REG_ADDRESS_CHANNEL_3_INPUT_RANGE			0x08
// Alarm Flag Registers (Read-Only)
#define ADS8694_REG_ADDRESS_ALARM_OVERVIEW_TRIPPED_FLAG		0x10
#define ADS8694_REG_ADDRESS_ALARM_CH_0_3_TRIPPED_FLAG		0x11
#define ADS8694_REG_ADDRESS_ALARM_CH_0_3_ACTIVE_FLAG		0x12
// Alarm Threshold Registers
#define ADS8694_REG_ADDRESS_CH_0_HYSTERESIS					0x15
#define ADS8694_REG_ADDRESS_CH_0_HIGH_THRESHOLD_MSB			0x16
#define ADS8694_REG_ADDRESS_CH_0_HIGH_THRESHOLD_LB			0x17
#define ADS8694_REG_ADDRESS_CH_0_LOW_THRESHOLD_MSB			0x18
#define ADS8694_REG_ADDRESS_CH_0_LOW_THRESHOLD_LSB			0x19
#define ADS8694_REG_ADDRESS_CH_1_HYSTERESIS					0x1A
#define ADS8694_REG_ADDRESS_CH_1_HIGH_THRESHOLD_MSB			0x1B
#define ADS8694_REG_ADDRESS_CH_1_HIGH_THRESHOLD_LB			0x1C
#define ADS8694_REG_ADDRESS_CH_1_LOW_THRESHOLD_MSB			0x1D
#define ADS8694_REG_ADDRESS_CH_1_LOW_THRESHOLD_LSB			0x1E
#define ADS8694_REG_ADDRESS_CH_2_HYSTERESIS					0x1F
#define ADS8694_REG_ADDRESS_CH_2_HIGH_THRESHOLD_MSB			0x20
#define ADS8694_REG_ADDRESS_CH_2_HIGH_THRESHOLD_LB			0x21
#define ADS8694_REG_ADDRESS_CH_2_LOW_THRESHOLD_MSB			0x22
#define ADS8694_REG_ADDRESS_CH_2_LOW_THRESHOLD_LSB			0x23
#define ADS8694_REG_ADDRESS_CH_3_HYSTERESIS					0x24
#define ADS8694_REG_ADDRESS_CH_3_HIGH_THRESHOLD_MSB			0x25
#define ADS8694_REG_ADDRESS_CH_3_HIGH_THRESHOLD_LB			0x26
#define ADS8694_REG_ADDRESS_CH_3_LOW_THRESHOLD_MSB			0x27
#define ADS8694_REG_ADDRESS_CH_3_LOW_THRESHOLD_LSB			0x28
// Command Read Back (Read-Only)
#define ADS8694_REG_ADDRESS_COMMAND_READ_BACK				0x3F

#define ADS8694_REG_AUTO_SEQ_CH3_EN							0x08
#define ADS8694_REG_AUTO_SEQ_CH2_EN							0x04
#define ADS8694_REG_AUTO_SEQ_CH1_EN							0x02
#define ADS8694_REG_AUTO_SEQ_CH0_EN							0x01
#define ADS8694_REG_CH3_POWER_DOWN							0x08
#define ADS8694_REG_CH2_POWER_DOWN							0x04
#define ADS8694_REG_CH1_POWER_DOWN							0x02
#define ADS8694_REG_CH0_POWER_DOWN							0x01

#define ADS8694_REG_RANGE_XPM_2_5							0x00
#define ADS8694_REG_RANGE_XPM_1_25							0x01
#define ADS8694_REG_RANGE_XPM_0_625							0x02
#define ADS8694_REG_RANGE_X2_5								0x05
#define ADS8694_REG_RANGE_X1_25								0x06


typedef struct {
	uint8_t byte[4];
} ADS8694_REG_BYTE;

typedef struct {
	uint16_t command;
	uint8_t data;
	uint8_t dout;
} ADS8694_REG_CMD_DATA;

typedef struct
{
    uint32_t    addr	: 7;
    uint32_t    wr		: 1;
    uint32_t    din		: 8;
    uint32_t	dout	: 8;
} ADS8694_REG_BIT;

typedef union {
	ADS8694_REG_BYTE byte;
	ADS8694_REG_CMD_DATA cmd_data;
	uint8_t data;
} ADS8694_REG;


typedef union {
	ADS8694_REG_BYTE ads8694_reg_byte;
	ADS8694_REG_CMD_DATA ads8694_reg_cmd_data;
} ADS8694_REG_UNION;


uint32_t ADS8694_Read(int channel){return 0;};

uint32_t ADS8694_WriteCommand(int channel){return 0;};

uint32_t ADS8694_WriteRegister(uint16_t data){return 0;};

int ADS8694_RegisterWriteRead(uint8_t* TxData, uint8_t* RxData){

	HAL_GPIO_WritePin(ADS8694_CS_PORT, ADS8694_CS_PIN, GPIO_PIN_RESET);

	if (HAL_SPI_TransmitReceive( \
			&hspi2, \
			TxData, \
			RxData, \
			(uint16_t)sizeof(ADS8694_REG_BYTE), \
			SPI2_TIMEOUT)!=HAL_OK) \
					return 1;
	HAL_GPIO_WritePin(ADS8694_CS_PORT, ADS8694_CS_PIN, GPIO_PIN_SET);
	return 0;

}


void ADS8694_Init(void) {

	HAL_GPIO_WritePin(ADS8694_CS_PORT, ADS8694_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ADS8694_RST_PORT, ADS8694_RST_PIN, GPIO_PIN_SET);

}
