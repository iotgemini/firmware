/*****************************************************************
	MCU: 					ATmega328p
	Programmer:				Emanuele Aimone
	Version:				1.0
	Last Update:			18 / 06 / 2023
	
	Description:			FW per IOTGEMINI platform
							
	
	License:				This firmware is provided in a way
							free and without any warranty
							The firmware can be modified and
							unlimited copied.
							The author does not assume any
							liability for damage brought or
							caused by this firmware.
							
	Licenza:				Questo firmware viene fornito in modo 
							gratuito e senza alcuna garanzia
							Il firmware pu� essere modificato e 
							copiato senza limiti.
							L'autore non si assume nessuna 
							responsabilit� per danni portati o
							causati da questo firmware.
							
*****************************************************************/

//HERE THE SETTINGS FOR THE FUSE OF ATMEGA328
//Remeber to also set into this file the define: #define F_CPU .......
/*
------------------------------------------------------
FUSES NORMAL PROGRAMMING:
	FUSES for 16MHz External clock with Crystal
		Low Fuse 		= 	0xFF
		High Fuse 		= 	0xDE
		Extended Fuse 	=	0xFD
		Lock Fuse 		= 	0xFF

	FUSES for 8MHz Internal clock
		Low Fuse 			= 	0xE2
		High Fuse 			= 	0xD9
		Extended Fuse 		= 	0xFE
		Lock Fuse 			= 	0xFF
------------------------------------------------------
------------------------------------------------------
FUSES WITH BOOTLOADER:
	FUSES 8MHz
		Low Fuse 	= 0xE2
		High Fuse 	= 0xDE
		Extended Fuse 	= 0xFE
		Lock Fuse 	= 0xFF
	
	FUSES 16MHz
	
		Low Fuse 	= 0xFF
		High Fuse 	= 0xDE
		Extended Fuse 	= 0xFE
		Lock Fuse 	= 0xFF
------------------------------------------------------
*/

#define F_CPU 16000000UL //this set the frequency used from the CPU to make works the library delay.h
//#define F_CPU 8000000UL

//*** DEFINE TO ENABLE/DISABLE FUNCTIONS
//#define UART_DEBUG	0	//debug on UART at 57600. 0=attiva fino allo strato fisico di comunicazione
//#define ENABLE_SPECIAL_FUNCTIONS


//#define FW_VERSION_RF_IOTGEMINI 				1	//this can go up to 65535. This will allow to reset the eeprom when the FW new is loaded on the MCU because it find on the eeprom a different version then will erase the eeprom
#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.2.2  11/04/2025		//into thresholds functions added the control that will overwrite the status of the output if some other control will change it
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.2.1  10/04/2025		//minor bugs
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.2.0  18/06/2023		//Added thresholds functions
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.1.6  19/03/2023
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.1.5
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.1.4
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.1.3
//#define FW_VERSION_RF_IOTGEMINI 				4	//v1.0.1.2
//#define FW_VERSION_RF_IOTGEMINI 				3	//v1.0.1.1
//#define FW_VERSION_RF_IOTGEMINI 				2	//v1.0.1.0
//#define FW_VERSION_RF_IOTGEMINI 				1	//v1.0.0.0

#define BAD_TRANSCEIVER		99	//

//#define RFNANO //this to use the arduino nano with integrated the nrf24l01 chip (under cosnstruction)

//uncomment the following line to get the macro works
//#define 	BLINK_LED			 	for(int ii=0;ii<5;ii++){PORTD|=0x80;_delay_ms(100);PORTD&=~0x80;_delay_ms(100);wdt_reset();}

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/sleep.h>


//Variabili globali:
volatile unsigned char sem_Used_As_Peripheral;
volatile unsigned char var_ID_TYPE_L,var_ID_TYPE_H;
volatile unsigned char var_EEPROM_Semaphore;
char var_carrier_led;


//Include funzioni:	 
#ifdef ENABLE_SPECIAL_FUNCTIONS
#include "./funzioni_speciali.h"
#endif

#include "./mcu/mcu_at328.h"

#include "./lib/RFPIMCU.h"
#include "./lib/list_lib_G3P.h"
#include "./lib_shields/manager.h"
#include "./lib_shields/basic_functions.h"
#include "./lib_shields/iotgemini_fun.h"

#include "./lib_shields/1_led.h" 			//ID=1
#include "./lib_shields/2_button.h" 		//ID=2
#include "./lib_shields/3_sensor.h" 		//ID=3
#include "./lib_shields/4_rgb.h" 			//ID=4
#include "./lib_shields/5_dht11.h" 			//ID=5

#ifdef UART_DEBUG
#include "./debug/debug.h"
#endif

#ifdef ENABLE_SPECIAL_FUNCTIONS
#include "./funzioni_speciali.c"
#endif

#include "./mcu/mcu_at328.c"

#include "./lib/RFPIMCU.c"
#include "./lib_shields/manager.c"
#include "./lib_shields/basic_functions.c"

#include "./lib_shields/1_led.c"
#include "./lib_shields/2_button.c"
#include "./lib_shields/3_sensor.c"
#include "./lib_shields/4_rgb.c" 	
#include "./lib_shields/5_dht11.c" 	
#include "./lib_shields/iotgemini_fun.c"

#ifdef UART_DEBUG
#include "./debug/debug.c"
#endif

#include "./mcu/interrupt_at328.c"


int main(void){
	wdt_reset(); //it reset the WDT
	unsigned char var_temp_byte;

	//this init the IO, init external modules, check protections
	init_mcu();

    //Ciclo infinito:
    do{ 
		//it goes out from the programming mode when the time elapsed
		if(programming_mode_enabled==1 && decont_programming_mode_enabled == 0){
			//set_current_network_Gateway(); //set the current network addreses
			#ifdef UART_DEBUG
			UART_DEBUG_send_STR(61, (unsigned char *)"TIME OUT FOR PROGRAMMING NETWORK, PREVIOUS ADRESSES RESTORED!",1); //it send a string through the UART only if is in debug mode
			#endif
			var_temp_byte = var_EEPROM_Semaphore | 0b00000001; //here disable the programming IoT network
			eeprom_write_byte ((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES), (unsigned char )var_temp_byte);
			PIN_LED_TX_ON; //led on
			var_RESET_NOW = 1; //at 1 it make the MCU to reset itself through the WDT
			while(1);
		}

		if( sem_Used_As_Peripheral == 1){
			//if used as peripheral then start the function for peripheral

			function_cmd_to_run();		//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt

			//here check if has been received 20byte from the UART, 
			//because 16bytes are the data and the first 4 bytes are the address of the transmitter (Gateway)
			//then it will parse the data received, the data obviously contain the command
			//and consecutively will execute the command			
			check_and_parse_cmd_from_Gateway();


			//here there are the functions which are executed autonomously by the peri	
			//example if an input change it will send the status to the Gateway
			/*if(decont_check_and_parse_functions_peri == 0){
				decont_check_and_parse_functions_peri = TIME_CHECK_AND_PARSE_FUNCTIONS_PERI; //this avoid to send too much data through radio
				check_and_parse_functions_peri();
			}*/
			
			check_and_parse_functions_manager(); //functions for the PIN to put into the infinite loop

		}else{ //here the code as transceiver only
			if(STATUS_RESET	== 0){
				while(STATUS_RESET	== 0); //it reset through the WDT
				var_RESET_NOW = 1; //at 1 it make the MCU to reset itself through the WDT
				while(1);
			}
			if( STATUS_CD == 0 ){
				PIN_LED_TX_ON;
			}else{
				PIN_LED_TX_OFF;
			}
			function_cmd_to_run();		//into the global variable CMD_to_Run there will be set the number of the command to run. Sometime command are executed immediately by the USART interrupt 
		}
		
		if(PIN_BTN_PROGRAMMING_MODE==LOGIC_BTN_PROGRAMMING_MODE){
			#ifdef UART_DEBUG
			UART_DEBUG_send_STR(13, (unsigned char *)"BTN0 PRESSED!",1); //it send a string through the UART only if is in debug mode
			#endif
			var_temp_byte = eeprom_read_byte((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES));
			if( (var_temp_byte&0b00000001) == 0b00000001){
				var_temp_byte &= 0b11111110; //here enable the programming IoT network
				#ifdef UART_DEBUG
				UART_DEBUG_send_STR(36, (unsigned char *)"Enabled the programming IoT network!",1); //it send a string through the UART only if is in debug mode
				#endif
			}else{
				var_temp_byte |= 0b00000001; //here disable the programming IoT network
				#ifdef UART_DEBUG
				UART_DEBUG_send_STR(37, (unsigned char *)"Disabled the programming IoT network!",1); //it send a string through the UART only if is in debug mode
				#endif
			}
			eeprom_write_byte ((unsigned char *)(START_BYTE_TO_CHECK_SEMAPHORES), (unsigned char )var_temp_byte);
			#ifdef UART_DEBUG
			UART_DEBUG_send_STR(23, (unsigned char *)"Going to reset ........",1); //it send a string through the UART only if is in debug mode
			#endif
			var_RESET_NOW = 1; //at 1 it make the MCU to reset itself through the WDT
			while(1);
		}

//		if(var_RESET_NOW == 0){
//			wdt_reset(); //it reset the WDT
//		}
		u8_decont_emulated_wdt = VALUE_EMULATE_WDT; //this variable has to be updated each time because at 0 the mcu would be rebooted
    }while(1);
	
	return 1;
}



