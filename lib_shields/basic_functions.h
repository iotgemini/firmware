/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			01 / 03 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Common functions for ATmega328
	
	
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
	//#define UART_DEBUG_BASIC_FUNCTIONS //activate the debug on the UART for this library
#endif

//Define for Arduino-IOTGEMINI Functions
#define OUTPUT							1	//
#define INPUT							0	//
#define INPUT_PULLUP					2
#define HIGH							1	//
#define LOW								0	//


//---------------------------------------------------------------------------------
//	BEGIN GLOBAL VARIABLES
//---------------------------------------------------------------------------------

volatile const unsigned char 	NUM_ADC0 = 0;
volatile const unsigned char 	NUM_MAX_SAMPLE_ADC0 = 8;
volatile unsigned int 			ADC0_Average;
volatile unsigned int 			ADC0_Average_Array[6];
volatile unsigned char 			contSampleADC0;

volatile const unsigned char 	NUM_ADC1 = 1;
volatile unsigned int 			ADC1_Average;

volatile const unsigned char 	NUM_ADC4 = 4;
volatile const unsigned char 	NUM_MAX_SAMPLE_ADC4 = 8;
volatile unsigned int 			ADC4_Average;
volatile unsigned int 			ADC4_Average_Array[6];
volatile unsigned char 			contSampleADC4;

volatile const unsigned char 	NUM_ADC5 = 5;
volatile const unsigned char 	NUM_MAX_SAMPLE_ADC5 = 8;
volatile unsigned int 			ADC5_Average;
volatile unsigned int 			ADC5_Average_Array[6];
volatile unsigned char 			contSampleADC5;

//---------------------------------------------------------------------------------
//	END GLOBAL VARIABLES
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	BEGIN LIST FUNCTIONS
//---------------------------------------------------------------------------------
void 			init_average_ADC(unsigned int *ADC_Average_Array, unsigned char *contSampleADC, unsigned char *NUM_MAX_SAMPLE_ADC, unsigned char *ADC_to_use);						//it init the average for ADC0
unsigned int 	get_average_ADC(unsigned int *ADC_Average_Array, unsigned char *contSampleADC, unsigned char *NUM_MAX_SAMPLE_ADC, unsigned char *ADC_to_use);							//salva il valore dell'ADC0 nell'array


void 			pinMode(unsigned char pin, unsigned char var_inout);		//Function Arduino-IOTGEMINI to set direction IO pin
void 			digitalWrite(unsigned char pin, unsigned char var_value);	//Function Arduino-IOTGEMINI to set value IO pin
unsigned char 	digitalRead(unsigned char pin);								//Function Arduino-IOTGEMINI to return value IO pin
void 			delayMicroseconds(double us);								//Function Arduino-IOTGEMINI
void 			enablePullUp(unsigned char pin);							//Function Arduino-IOTGEMINI to enable Pull Up resistor
void 			disablePullUp(unsigned char pin);							//Function Arduino-IOTGEMINI to disable Pull Up resistor


#ifdef UART_DEBUG_BASIC_FUNCTIONS
void 	UART_DEBUG_BASIC_FUNCTIONS_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed);//it send a string through the UART only if is in debug mode
#endif
//---------------------------------------------------------------------------------
//	END LIST FUNCTIONS
//---------------------------------------------------------------------------------
