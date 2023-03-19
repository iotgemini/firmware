/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			28 / 02 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Functions for sensor
	
	
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

#ifdef UART_DEBUG
	//#define UART_DEBUG_SENSOR_3 //activate the debug on the UART for this library
#endif

//#define ID_SHIELD_H				0		//it is the most significant byte of the univocal identifier  
//#define ID_SHIELD_L				1		//it is the less significant byte of the univocal identifier  


#define NUM_IO_SENSOR_3			1		//it is the less significant byte of the univocal identifier

//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------

struct SENSOR_3 {
  char num_io;
  char io[2];
  int year;
};

//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------
	//here the list of the functions to be edited to implement the features wanted														
	void 			init_SENSOR_3(struct settings_shield *current_settings_shield);					//Function that allocate the variables and init the shield
	
	
	#ifdef UART_DEBUG_SENSOR_3
	void 	UART_DEBUG_SENSOR_3_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed);//it send a string through the UART only if is in debug mode
	void 	UART_DEBUG_SENSOR_3_send_STR2(unsigned char *data, unsigned char line_feed);
	#endif
//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
