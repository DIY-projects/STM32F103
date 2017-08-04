#include "stm32f1xx_hal.h"
#include "user_code.h"
#include <string.h>

const char msg_Greeting[] = "4QPS\n";


int msg_to_client(char* m) {
	if (HAL_SPI_Transmit(&hspi2, (uint8_t*)m, strlen(m)+1, 100)==HAL_OK)
		return 0;
	else
		return 1;
}



int link_getc(uint8_t* c) {
	if (HAL_SPI_Receive(&hspi2, c, 1, 100)==HAL_OK) {
		return 0;  // a character has been received
	}
	else
		return 1;  // nothing received
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
				 *(message+strlen(message))=ch;
			 	 *(message+strlen(message)+1)='\0';
			 }
			 break;
		 case 'V':
//			 if (strchr(message,'.')==strlen(message)) *(message+strlen(message)-1)='\0';
			 return set_voltage;
		 case 'A':
//			 if (strchr(message,'.')==strlen(message)) *(message+strlen(message)-1)='\0';
			return set_current;
		 default:
			 *message='\0';
			 break;
		}
	return no_command;	//	no command received
}


int init_gui_client(const char* msg) {
char sbuf[MSG_BUFFER_SIZE];
if (!strcpy(&sbuf[0],msg))
	if (!msg_to_client(&sbuf[0])) {
		int retry;
		for (retry=0;retry<5;retry++) {
			if (!msg_from_client(&sbuf[0])) {
				if (strcmp(&sbuf[0],"OK"))
						return 0;		// valid reponse from client
			}
		}
	}
	return 1;
}


int msg_loop() {
	char msg_buf[MSG_BUFFER_SIZE];
	msg_buf[0]='\0';

	while (1) {
		switch (msg_from_client(&msg_buf[0])) {
			case no_command:
					break;
			case set_voltage:
			case set_current:
			case read_voltage:
			case read_current:
				if (msg_to_client(&msg_buf[0])) return 1; // exit if error sending msg to client
				msg_buf[0]='\0';
				break;
			default:
				break;
		}
	// *********************************************** Do other things in the loop here
	LED_BLINK(300);
	// *********************************************** End of loop
	}
}


int user_code_entry() {
	LED_OFF(100);
//	LED_BLINK;

//	if (!init_gui_client(&msg_Greeting[0])) {
		// 2 blinks on LED
//		LED_BLINK;
//		LED_BLINK;
		msg_loop();  // should stay in the infinite loop
//	}
return 1;  // error occured
}


// *************************************** Unused code below

//*************************************************************************** TESTING RELAYS
//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);  // toggle relay
// 	    HAL_Delay(100);
//	    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);  // toggle relay
	/* Insert delay 100 ms */
