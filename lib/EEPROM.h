/*****************************************************************

	MCU: 					ATmega328
	Autore: 				Emanuele Aimone
	Last Update:			01 / 03 / 2023
	
	Descrizione:			Libreria per scrivere/leggere i dati 
							nella/dalla EEPROM. I dati provenienti 
							da un array vengono scritti sulle 
							celle della EEPROM.
	
	License:				This firmware is provided in a way
							free and without any warranty
							The firmware can be modified and
							unlimited copied.
							The author does not assume any
							liability for damage brought or
							caused by this firmware.

	Licenza:				Questo firmware viene fornito in modo 
							gratuito e senza alcuna garanzia
							Il firmware può essere modificato e 
							copiato senza limiti.
							L'autore non si assume nessuna 
							responsabilità per danni portati o
							causati da questo firmware.
							
*****************************************************************/


#define 	START_BYTE_TO_CHECK_SEMAPHORES						0x0F	//this byte is used to record informations like if is pressed the BTN0

#define 	START_ADDRESS_WHERE_TO_SAVE_NETWORK					0x10	//in the first 7 addresses there are saved the addresses of the RFPI network

#define 	START_ADDRESS_FREE									0x20 	//

#define 	START_ADDRESS_FW_VERSION							0x26 	//here is written the release of the firmware

#define 	START_ADDRESS_WHERE_TO_SAVE_IO_STATUS				0x30 	//here are saved all IO status and variables for the peri

#define 	START_ADDRESS_WHERE_TO_SAVE_RF						0x80 	//

#define 	START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD		0xD0 	//96 bytes. here are saved the ID of the shields to load the configurations

#define 	START_ADDRESS_OUT_PIN_VALUE							0x130	//1 byte. id_data_eeprom_to_update = 0b000000000000001. inside there is the last status of the output pins

#define 	START_ADDRESS_PWM_VALUE								0x131 	//3 bytes. id_data_eeprom_to_update = 0b0000000000111000. here are saved the pwm value

#define 	START_ADDRESS_TIMER_RELAY_VALUE						0x134 	//4 bytes. id_data_eeprom_to_update = 0b0000000001000000. here are saved the value for semRelayTimer,RelayTimer_SS,RelayTimer_MM,RelayTimer_HH; 

																//0x138	//8bytes FREE

#define 	START_ADDRESS_SETTINGS_PLATFORM						0x140	//1 byte. id_data_eeprom_to_update = 0b0000000100000000. This used to keep off the Led TX (led blue)

#define 	START_ADDRESS_TIMER_RELAY_VALUE_10MS				0x141 	//1 bytes. id_data_eeprom_to_update = 0b0000000001000000. here are saved the value for RelayTimer_10ms

#define 	START_ADDRESS_INPUTS_DUTY_VALUE						0x142 	//10 bytes. fun_input_ctrl_output[8] where are saved the value for the functions Inputs Duty

#define 	START_ADDRESS_THRESHOLD_VALUES						0x14C 	//10 bytes. id_data_eeprom_to_update = 0b0000001000000000.


#define 	VALUE_VIRGIN_BYTE					0xFF

void save_array_data_EEPROM(unsigned char *array_data, unsigned int lenght_array_data, unsigned int var_START_ADDRESS_WHERE_TO_SAVE);
unsigned char return_array_data_EEPROM(unsigned char *array_data, unsigned int lenght_array_data, unsigned int var_START_ADDRESS_WHERE_TO_SAVE);
