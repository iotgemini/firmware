/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			28 / 02 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Common functions to use umidity sensor DHT11
	
	
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
	//#define UART_DEBUG_DHT11_5 //activate the debug on the UART for this library
#endif


//#define ID_SHIELD_H				0		//it is the most significant byte of the univocal identifier  
//#define ID_SHIELD_L				5		//it is the less significant byte of the univocal identifier  


#define NUM_IO_DHT11_5			1		//it is the less significant byte of the univocal identifier

#define DHTLIB_OK                0
//#define DHTLIB_ERROR_CHECKSUM   -1
//#define DHTLIB_ERROR_TIMEOUT    -2

//#define DHT11_DELAY_MS_BEFORE_READ_AGAIN    2500 	//it determine the reading frequency, for the DHT22 the max frequency should be at maximum of 0.5Hz so this as to be set more than 2000ms


//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------



struct DHT11_5 {
  unsigned int temperature[8];
  unsigned int I_umidity[8]; //integer part
 // unsigned int D_umidity[8]; //decimal part
  //unsigned int checkSum[8]; //integer part
  unsigned char sem_get_new_value_with_this_pin;
  //unsigned int decont_delay_ms_before_read_again;
};

volatile struct DHT11_5 *pointer_struct_DHT11;



//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------

	//here the list of the functions to be edited to implement the features wanted														
	void 			init_DHT11_5(struct settings_shield *current_settings_shield);					//Function that allocate the variables and init the shield
	
	unsigned char 	dht11_read(unsigned char wheight_pin);
	
	//void 			decont_vars_DHT11(void);														//this function has to be called every 1mS, so has to be written into the interrupt routine of 1mS
	
	//unsigned char read_DHT11(unsigned char force);
		
	
	#ifdef UART_DEBUG_DHT11_5
	void 	UART_DEBUG_DHT11_5_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed);//it send a string through the UART only if is in debug mode
	void 	UART_DEBUG_DHT11_5_send_STR2(unsigned char *data, unsigned char line_feed);
	#endif
//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
