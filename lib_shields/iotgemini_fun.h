/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			01 / 03 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			IOTGEMINI platform functions for ATmega328
	
	
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

//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------
//Function that return number of the pin (from pin3 to pin10) from the number of the ID
unsigned char num_pin_iotgemini_from_id_input(unsigned char id_io);

//Function that return number of the pin (from pin3 to pin10) from the number of the ID
unsigned char num_pin_iotgemini_from_id_output(unsigned char id_io);
	
//this get the value and into data[4] there is a value on 8bit, into the data[5] there is the num of bit used,
//if the num bit used is over 8bit the data it is not kept into data[4], but into the consecutive data[6],data[7].....data[x]
unsigned char return_value_pin(unsigned char num_pin_to_return_status, unsigned char *data);

//translate the weight of the pin IOTGEMINI to the address of the register of the real pin on the AT328
void return_AT328_io_register_from_IOTG_pin( unsigned char wheight_pin, volatile uint8_t *port_register, volatile uint8_t *direction_register, volatile uint8_t *portin_register, volatile uint8_t *weight_pin_AT328 );
	
//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
