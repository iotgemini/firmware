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


//***********************************************************************************
//	HERE START THE FUNCTIONS TO BE EDITED TO IMPLEMENT THE FEATURES WANTED
//***********************************************************************************

//Function that allocate the variables and init the shield
void 	init_RGB_4(struct settings_shield *currentSettingsShield){
	int dt_RED, dt_GREEN, dt_BLUE;
	
	//the manager init the pin as input thus I have just to add +1 to the variables var_NUM_ANALOGUE_INPUT
	var_NUM_ANALOGUE_OUTPUT+=3;

	root_pin_functions->SEM_PWM |= 0b01001001;
	root_pin_functions->PIN_MASK |= root_pin_functions->SEM_PWM;
	init_RGB_LED_PWM();

	dt_RED = (int)eeprom_read_byte((unsigned char *)START_ADDRESS_PWM_VALUE);
	dt_GREEN = (int)eeprom_read_byte((unsigned char *)START_ADDRESS_PWM_VALUE+1);
	dt_BLUE = (int)eeprom_read_byte((unsigned char *)START_ADDRESS_PWM_VALUE+2);
	
	//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
	set_duty_cycle_RGB_LED(dt_RED, dt_GREEN, dt_BLUE);

	#ifdef UART_DEBUG_RGB_4
	UART_DEBUG_MANAGER_send_STR2((unsigned char *)"RGB SET!",1); //it send a string through the UART only if is in debug mode
	//UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)root_pin_functions->SEM_ADC, 0);
	#endif
}


void init_RGB_LED_PWM(void){ 	//inizializza i registri del PWM su PD3, PD5 and PD6 per RGB LED
								//PB1 = Red
								//PB2 = Green
								//PB3 = Blue
	//PWM1&2 initialisation
	/*TCCR1A = 0b10100001;// set PWM mode for PB1 and PB2
	TCCR1B = 0b00000010; //set clock:8 prescaler

	//PWM3 initialisation
	TCCR2A = 0b10000001; // set PWM mode for PB3
	TCCR2B = 0b00000010; //set clock:8 prescaler
	*/
	
	//PD3 = Red
	//PD5 = Green
	//PD6 = Blue
	
	//PWM1&2 initialisation
	TCCR0A = 0b10100001;// set PWM mode for PD5 and PD6
	TCCR0B = 0b00000010; //set clock:8 prescaler

	//PWM3 initialisation
	TCCR2A = 0b00100001; // set PWM mode for PD3
	TCCR2B = 0b00000010; //set clock:8 prescaler							
}


//it set the duty cycle for the PWM on PD3, PD5 and PD6 for RGB LED
void set_duty_cycle_RGB_LED(int dt_RED, int dt_GREEN, int dt_BLUE){
	//PB1 = Red
	//PB2 = Green
	//PB3 = Blue
	/*OCR1A = dt_RED; 
	OCR1B = dt_GREEN;
	OCR2A = dt_BLUE;*/
	
	//PD3 = Red
	//PD5 = Green
	//PD6 = Blue
	if(dt_BLUE!=-1)	OCR0A = (unsigned char)dt_BLUE; 
	if(dt_GREEN!=-1)OCR0B = (unsigned char)dt_GREEN;
	if(dt_RED!=-1)	OCR2B = (unsigned char)dt_RED;
}


unsigned char get_duty_cycle_RED_LED(void){
	return (unsigned char)OCR2B; //8bit
}
unsigned char get_duty_cycle_GREEN_LED(void){
	return (unsigned char)OCR0B; //8bit
}
unsigned char get_duty_cycle_BLUE_LED(void){
	return (unsigned char)OCR0A; //8bit
}
