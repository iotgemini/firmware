/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			28 / 02 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Functions for RGB shield
	
	
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
	//#define UART_DEBUG_RGB_4 //activate the debug on the UART for this library
#endif


//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------
	//here the list of the functions to be edited to implement the features wanted														
	void 			init_RGB_4(struct settings_shield *current_settings_shield);	//Function that allocate the variables and init the shield
	
	void 			init_RGB_LED_PWM(void); 										//inizializza i registri del PWM su PD3, PD5 and PD6 per RGB LED
	
	void set_duty_cycle_RGB_LED(int dt_RED, int dt_GREEN, int dt_BLUE);	//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
	
	unsigned char get_duty_cycle_RED_LED(void);
	unsigned char get_duty_cycle_GREEN_LED(void);
	unsigned char get_duty_cycle_BLUE_LED(void);

//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
