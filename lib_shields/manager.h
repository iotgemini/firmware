/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			01 / 03 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			It is the manager to load the 
							configurations of the shields
	
	
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
	#define UART_DEBUG_MANAGER //activate the debug on the UART for this library
#endif


#define MAX_ID_EXISTING_SHIELDS					8		//

#define MAX_NUM_SHIELDS_CONNECTABLE				8		//

#define NUM_BYTE_EEPROM_USED_BY_EACH_SHIELD		12		//

#define MAX_DELAY_RUN_MANAGER_FUNCTION			15 		//dato moltipliacato per 10. con valore di 5 allora ogni circa 50mS esegue una funzione

//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------


volatile struct settings_shield {
	
	volatile unsigned int ID;
	volatile unsigned char PIN_USED;
	volatile unsigned char PIN_MASK;
	volatile unsigned char PULL_UP;
	volatile unsigned char array_ID_Functions[8];

	volatile struct settings_shield	*next;
	
} struct_settings_shield;

	
volatile struct manager_shield {
	
  volatile unsigned char num_shield_linked;
  
  volatile struct settings_shield *root_settings_shield; 

//EEPROM MAP:
//the EEPROM address where start the configurations is kept by START_ADDRESS_WHERE_TO_CONFIGURATIONS_SHIELD
//the first byte is the number of shield loaded, thus it is assigned to the variable num_shield_linked
//the following bytes are the ID shield and function to load

// 	byte 0 	=	num_shield_linked
//	byte 1-2=	array_ID_shields[0]			//Byte1=H e Byte2=L
//	byte 3 	=	array_PIN_USED_shield[0]
//	byte 4 	=	array_PIN_MASK_shield[0]
//	byte 5 	=	matrix_functions_shields[0][0]	//if the byte is 0 then it does not load anythings
//	byte 6 	=	matrix_functions_shields[0][1]
//	byte 7 	=	matrix_functions_shields[0][2]
//    ..............................
//	byte 14	=	matrix_functions_shields[0][8]

//	byte 15 =	array_ID_shields[1]			//Byte1=H e Byte2=L
//	byte 16	=	array_PIN_USED_shield[1]
//	byte 17	=	array_PIN_MASK_shield[1]
//  byte 18 =	matrix_functions_shields[1][0]
//  byte 19 =	matrix_functions_shields[1][1]
//    ..............................
//  byte 27 =	matrix_functions_shields[1][8]

//    ..............................
//    ..............................

//	byte 92 =	array_ID_shields[7]			//Byte1=H e Byte2=L
//	byte 93	=	array_PIN_USED_shield[7]
//	byte 94	=	array_PIN_MASK_shield[7]
//  byte 95 =	matrix_functions_shields[7][0]
//  byte 96 =	matrix_functions_shields[7][1]
//    ..............................
//  byte 105 =	matrix_functions_shields[7][8]
	
} struct_manager_shield;


//semaphores for the PIN functions
volatile struct pin_functions {
	
	volatile unsigned char PIN_USED;	//this are the pins used by the shields
	
	volatile unsigned char PIN_MASK;	//this used to know if are input or output. 
										//0=input, 1=output
										//bit0=PIN3 ...... bit7=PIN10
																		
	volatile unsigned char PIN_STATUS;	//this used to know the status of the pin  
										//0=low, 1=high
										//bit0=PIN3 ...... bit7=PIN10
	
	
	volatile unsigned char SEM_ADC;		//bit0 = semaphore for ADC0
										//bit4 = semaphore for ADC4
										//bit5 = semaphore for ADC5
										
	volatile unsigned char SEM_PWM;		//bit0 = semaphore for PWM_R
										//bit3 = semaphore for PWM_G
										//bit6 = semaphore for PWM_B
	
	volatile unsigned char SEM_UART;	



	volatile unsigned char SEM_I2C;	
	
	
	
	volatile unsigned char SEM_DHT11;
	
	
	
	
} struct_pin_functions;



volatile struct manager_shield *root_struct_shields;
volatile struct settings_shield *current_settings_shield;
volatile struct pin_functions *root_pin_functions;

	
volatile unsigned char cont_Timer_Manager_Shield_HH;
volatile unsigned char cont_Timer_Manager_Shield_MM;
volatile unsigned char cont_Timer_Manager_Shield_SS;
volatile unsigned char cont_Timer_Manager_Shield_ms;
volatile unsigned char cont_Timer_Manager_Shield_10ms;


//counter
volatile unsigned char decont_check_and_parse_functions_manager;
volatile unsigned char last_cont_10ms_function_manager;


//variable that make the infinite loop to execute for every cycle only one function per time
volatile unsigned char ID_Function_Manager_to_Run;


//variables used into lib RFPIMCU
volatile unsigned char var_NUM_DIGITAL_INPUT;
volatile unsigned char var_NUM_DIGITAL_OUTPUT;
volatile unsigned char var_NUM_ANALOGUE_INPUT;
volatile unsigned char var_NUM_ANALOGUE_OUTPUT;
volatile unsigned char var_NUM_FUNCTIONS;

//this variables make to update the eeprom into the function check_and_parse_functions_manager()
volatile unsigned int id_data_eeprom_to_update;


//BEGIN VARIABLES USED FOR TIMER OUTPUT
//used for the timer that control one of the output
volatile unsigned char 	semRelayTimer,RelayTimer_SS,RelayTimer_MM,RelayTimer_HH;
volatile unsigned char 	RelayTimer_10ms;
volatile unsigned char 	decont_semRelayTimer,decont_RelayTimer_MM,decont_RelayTimer_HH;
volatile signed char decont_RelayTimer_SS;
volatile unsigned char 	decont_RelayTimer_10ms;
volatile unsigned char last_cont_Timer_Manager_Shield_SS;
volatile unsigned char last_cont_Timer_Manager_Shield_10ms;
//END VARIABLES USED FOR TIMER OUTPUT

//used for the function 2 that is Inputs Duty
volatile unsigned char fun_input_ctrl_output[10];
volatile unsigned char last_status_input;

//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------
void 			manager_load_shields(void);					//Function that read the settings into the eeprom and then load the shields configurations

void 			check_and_parse_functions_manager_shield_every_1mS(void); //here some functions that needs to be run every 1mS

void 			check_and_parse_functions_manager(void); //functions for the PIN to put into the infinite loop

void 			lib_shields(struct settings_shield *current_shield_settings, unsigned char function_to_run); //functions of the shields libraries


#ifdef UART_DEBUG_MANAGER
void 	UART_DEBUG_MANAGER_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed);//it send a string through the UART only if is in debug mode
void 	UART_DEBUG_MANAGER_send_STR2(unsigned char *data, unsigned char line_feed);

#endif
//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
