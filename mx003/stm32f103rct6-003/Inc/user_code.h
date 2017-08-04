/*
 * user_code.h
 *
 *  Created on: 2017¦~8¤ë4¤é
 *      Author: ming
 */

#ifndef USER_CODE_H_
#define USER_CODE_H_

#define MSG_BUFFER_SIZE	20

#define LED_PIN		GPIO_PIN_8
#define LED_PORT	GPIOC

#define LED_ON(X)		{HAL_GPIO_WritePin(LED_PORT, LED_PIN,0);HAL_Delay(X);}
#define LED_OFF(X)		{HAL_GPIO_WritePin(LED_PORT, LED_PIN,1);HAL_Delay(X);}
//#define LED_BLINK	{LED_ON; HAL_Delay(300);LED_OFF; HAL_Delay(300);}
#define LED_BLINK(Y)	{LED_ON(Y);LED_OFF(Y);}

typedef enum {
	no_command = 0,
	set_voltage,
	set_current,
	read_voltage,
	read_current,
	max_command
} type_client_cmd;

int link_getc(uint8_t*);
int msg_loop(void);
int msg_to_client(char*);
type_client_cmd msg_from_client(char*);
int user_code_entry(void);
int init_gui_client(const char*);


extern SPI_HandleTypeDef hspi2;

#endif /* USER_CODE_H_ */
