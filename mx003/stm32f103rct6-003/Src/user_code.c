#include "stm32f1xx_hal.h"
#include "user_code.h"
#include "DAC80004.h"
#include "ADS8694.h"
#include <string.h>
#include <stdio.h>

#define MSG_BUFFER_SIZE	20

#define LED_PIN		GPIO_PIN_8
#define LED_PORT	GPIOC

#define LED_ON(X)		{HAL_GPIO_WritePin(LED_PORT, LED_PIN,GPIO_PIN_SET);HAL_Delay(X);}
#define LED_OFF(X)		{HAL_GPIO_WritePin(LED_PORT, LED_PIN,GPIO_PIN_RESET);HAL_Delay(X);}
//#define LED_BLINK	{LED_ON; HAL_Delay(300);LED_OFF; HAL_Delay(300);}
#define LED_BLINK(Y)	{LED_ON(Y);LED_OFF(Y);}


#define RELAY_K4_PORT	GPIOC
#define RELAY_K4_PIN	GPIO_PIN_14
#define RELAY_K1_PORT	GPIOC
#define RELAY_K1_PIN	GPIO_PIN_13
#define RELAY_K2_PORT	GPIOB
#define RELAY_K2_PIN	GPIO_PIN_9
#define RELAY_K3_PORT	GPIOB
#define RELAY_K3_PIN	GPIO_PIN_8


#define	RELAY_72V	HAL_GPIO_WritPin(RELAY_K1_PORT, RELAY_K1_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K2_PORT, RELAY_K2_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K3_PORT, RELAY_K3_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K4_PORT, RELAY_K4_PIN,GPIO_PIN_SET);

#define	RELAY_30V	HAL_GPIO_WritPin(RELAY_K1_PORT, RELAY_K1_PIN,GPIO_PIN_SET); \
					HAL_GPIO_WritPin(RELAY_K2_PORT, RELAY_K2_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K3_PORT, RELAY_K3_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K4_PORT, RELAY_K4_PIN,GPIO_PIN_RESET);

#define	RELAY_15V	HAL_GPIO_WritPin(RELAY_K1_PORT, RELAY_K1_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K2_PORT, RELAY_K2_PIN,GPIO_PIN_SET); \
					HAL_GPIO_WritPin(RELAY_K3_PORT, RELAY_K3_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K4_PORT, RELAY_K4_PIN,GPIO_PIN_RESET);

#define	RELAY_8V	HAL_GPIO_WritPin(RELAY_K1_PORT, RELAY_K1_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K2_PORT, RELAY_K2_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K3_PORT, RELAY_K3_PIN,GPIO_PIN_RESET); \
					HAL_GPIO_WritPin(RELAY_K4_PORT, RELAY_K4_PIN,GPIO_PIN_SET);




typedef enum {
	no_command = 0,
	set_voltage,
	set_current,
	read_voltage,
	read_current,
	read_temperature,
	max_command
} type_client_cmd;

int temperature(char* tp) {
//**************************************************************************************
/****** add "-u _printf_float" to Linker Flags to print floating points with sprintf****/
//**************************************************************************************
	uint16_t AD_Value = 0;
	if (HAL_ADC_Start(&hadc1)!=HAL_OK) return 1;
	if (HAL_ADC_PollForConversion(&hadc1,20) != HAL_OK) return 1;
	AD_Value = HAL_ADC_GetValue(&hadc1);
	if (0>sprintf(tp,"%6.2f",(((float)AD_Value/4096.f*3.3f-0.5f)/0.01f))) return 1 ;
	if (HAL_ADC_PollForConversion(&hadc1,20) != HAL_OK) return 1;
	AD_Value = HAL_ADC_GetValue(&hadc1);
	if (0>sprintf(tp+strlen(tp),",%6.2f",(((float)AD_Value/4096.f*3.3f-0.5f)/0.01f))) return 1;
	if (HAL_ADC_Stop(&hadc1)!=HAL_OK) return 1;
	return 0;
}

int msg_to_client(char* m) {
	int status = HAL_UART_Transmit(&huart1, (uint8_t*)m, strlen(m)+1, 100);
	*m='\0';
	return (status!=HAL_OK);
}


int link_getc(uint8_t* c) {
	return (HAL_UART_Receive(&huart1, c, 1, 100)!=HAL_OK);
}


type_client_cmd msg_from_client(char* message) {
	char ch='\0';
	if (!link_getc((uint8_t*)&ch))
		/* got a charater */
		switch (ch) {
		 case '.': if (strchr(message,'.')) break;
		 case '0': if (strlen(message)==0) break;
		 case '1':
		 case '2':
		 case '3':
		 case '4':
		 case '5':
		 case '6':
		 case '7':
		 case '8':
		 case '9':
			 if (strlen(message)<(MSG_BUFFER_SIZE-1)) {
//				 int len=strlen(message);
//				 *(message+len)=ch;
//				 *(message+len+1)='\0';

				 *(message+strlen(message)+1)=(char)'\0';  // These two lines must be in this order
			 	 *(message+strlen(message))=(char)ch;  // or otherwise strlen values will be incorrect.
			 }
			 break;
		 case 'V':
			 return set_voltage;
		 case 'v':
			return read_voltage;
		 case 'A':
			return set_current;
		 case 'a':
			return read_current;
		 case 'T':
			return read_temperature;
		 default:
			 *message='\0';
			 break;
		}
	return no_command;	//	no command received
}



int msg_loop() {
	char msg_buf[MSG_BUFFER_SIZE];
	msg_buf[0]='\0';

	while (1) {
		switch (msg_from_client(&msg_buf[0])) {
			case no_command:
					break;
			case set_voltage:
				if (DAC80004_Set(1,2, 3, 4, 1)) return 1;
			case set_current:
			case read_voltage:
			case read_current:
				if (msg_to_client(&msg_buf[0])) return 1; // exit if error sending msg to client
				msg_buf[0]='\0';
				break;
			case read_temperature:
				if (temperature(&msg_buf[0])) return 1;
				if (msg_to_client(&msg_buf[0])) return 1; // exit if error sending msg to client
				msg_buf[0]='\0';
				break;
			default:
				break;
		}
	// *********************************************** Do other things in the loop here
	//LED_BLINK(300);
	// *********************************************** End of loop
	}
}


int user_code_entry() {
	__HAL_SPI_ENABLE(&hspi2);
	DAC80004_Init();
	ADS8694_Init();
	msg_loop();  // should stay in the infinite loop
//	}
return 1;  // error occured
}

