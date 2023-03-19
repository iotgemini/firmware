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


//***********************************************************************************
//	HERE START THE FUNCTIONS TO BE EDITED TO IMPLEMENT THE FEATURES WANTED
//***********************************************************************************

//it init the average for ADC
void  init_average_ADC(unsigned int *ADC_Average_Array, unsigned char *contSampleADC, unsigned char *NUM_MAX_SAMPLE_ADC, unsigned char *ADC_to_use){
	unsigned char i;
	for(i=0;i<*NUM_MAX_SAMPLE_ADC;i++){
		get_average_ADC(ADC_Average_Array, contSampleADC, NUM_MAX_SAMPLE_ADC, ADC_to_use);
	}
}


//salva il valore dell'ADC nell'array
unsigned int get_average_ADC(unsigned int *ADC_Average_Array, unsigned char *contSampleADC, unsigned char *NUM_MAX_SAMPLE_ADC, unsigned char *ADC_to_use){
	unsigned int value_adc;
	value_adc = get_10bit_adc(*ADC_to_use); //ADC
	return value_adc;
}



//Function Arduino-IOTGEMINI to set direction IO pin
void pinMode(unsigned char pin, unsigned char var_inout){
	if( var_inout == INPUT_PULLUP ){//set input with pull-up
		enablePullUp(pin);
	}
			
	if( (pin & 0b00000001) != 0 ){//PIN3
		if( var_inout == OUTPUT ){// set as output
			PIN3_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN3 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN3_SET_AS_INPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN3 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00000010) != 0 ){//PIN4
		if( var_inout == OUTPUT ){// set as output
			PIN4_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN4 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN4_SET_AS_INPUT;
			//PIN4_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN4 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00000100) != 0 ){//PIN5
		if( var_inout == OUTPUT ){// set as output
			PIN5_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN5 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN5_SET_AS_INPUT;
			//PIN5_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN5 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00001000) != 0 ){//PIN6
		if( var_inout == OUTPUT ){// set as output
			PIN6_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN6 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN6_SET_AS_INPUT;
			//PIN6_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN6 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00010000) != 0 ){//PIN7
		if( var_inout == OUTPUT ){// set as output
			PIN7_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN7 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN7_SET_AS_INPUT;
			//PIN7_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN7 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00100000) != 0 ){//PIN8
		if( var_inout == OUTPUT ){// set as output
			PIN8_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN8 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN8_SET_AS_INPUT;
			//PIN8_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN8 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b01000000) != 0 ){//PIN9
		if( var_inout == OUTPUT ){// set as output
			PIN9_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN9 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN9_SET_AS_INPUT;
			//PIN9_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN9 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b10000000) != 0 ){//PIN10
		if( var_inout == OUTPUT ){// set as output
			PIN10_SET_AS_OUTPUT;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN10 OUT",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set as input
			PIN10_SET_AS_INPUT;
			//PIN10_DISABLE_PULLUP;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN10 IN",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}
	
}




//Function Arduino-IOTGEMINI to set value IO pin
void digitalWrite(unsigned char pin, unsigned char var_value){
	if( (pin & 0b00000001) != 0 ){//PIN3
		if( var_value != 0 ){// set ON
			PIN3_SET_ON; 
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN3 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN3_SET_OFF; 
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN3 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00000010) != 0 ){//PIN4
		if( var_value != 0 ){// set as output
			PIN4_SET_ON; 
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN4 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN4_SET_OFF; 
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN4 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00000100) != 0 ){//PIN5
		if( var_value != 0 ){// set ON
			PIN5_SET_ON; 
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN5 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN5_SET_OFF; 
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN5 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00001000) != 0 ){//PIN6
		if( var_value != 0 ){// set ON
			PIN6_SET_ON;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN6 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN6_SET_OFF;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN6 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00010000) != 0 ){//PIN7
		if( var_value != 0 ){// set ON
			PIN7_SET_ON;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN7 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN7_SET_OFF;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN7 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b00100000) != 0 ){//PIN8
		if( var_value != 0 ){// set ON
			PIN8_SET_ON;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN8 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN8_SET_OFF;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN8 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b01000000) != 0 ){//PIN9
		if( var_value != 0 ){// set ON
			PIN9_SET_ON;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN9 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN9_SET_OFF;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN9 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}else if( (pin & 0b10000000) != 0 ){//PIN10
		if( var_value != 0 ){// set ON
			PIN10_SET_ON;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN10 ON",1); //it send a string through the UART only if is in debug mode
			#endif
		}else{ //set OFF
			PIN10_SET_OFF;
			#ifdef UART_DEBUG_BASIC_FUNCTIONS
			UART_DEBUG_BASIC_FUNCTIONS_send_STR((unsigned char *)"  PIN10 OFF",1); //it send a string through the UART only if is in debug mode
			#endif
		}
	}
	
}


//Function Arduino-IOTGEMINI to return value IO pin
unsigned char 	digitalRead(unsigned char pin){
	unsigned char value_pin=0;
	if( (pin & 0b00000001) != 0 ){//PIN3
		value_pin = STATUS_PIN3; 
	}else if( (pin & 0b00000010) != 0 ){//PIN4
		value_pin = STATUS_PIN4; 
	}else if( (pin & 0b00000100) != 0 ){//PIN5
		value_pin = STATUS_PIN5; 
	}else if( (pin & 0b00001000) != 0 ){//PIN6
		value_pin = STATUS_PIN6; 
	}else if( (pin & 0b00010000) != 0 ){//PIN7
		value_pin = STATUS_PIN7; 
	}else if( (pin & 0b00100000) != 0 ){//PIN8
		value_pin = STATUS_PIN8; 
	}else if( (pin & 0b01000000) != 0 ){//PIN9
		value_pin = STATUS_PIN9; 
	}else if( (pin & 0b10000000) != 0 ){//PIN10
		value_pin = STATUS_PIN10; 
	}
	
	return value_pin;
}						


//Function Arduino-IOTGEMINI to enable Pull Up resistor
void enablePullUp(unsigned char pin){
	if( (pin & 0b00000001) != 0 ){//PIN3
		PIN3_ENABLE_PULLUP; 
	}else if( (pin & 0b00000010) != 0 ){//PIN4
		PIN4_ENABLE_PULLUP; 
	}else if( (pin & 0b00000100) != 0 ){//PIN5
		PIN5_ENABLE_PULLUP; 
	}else if( (pin & 0b00001000) != 0 ){//PIN6
		PIN6_ENABLE_PULLUP; 
	}else if( (pin & 0b00010000) != 0 ){//PIN7
		PIN7_ENABLE_PULLUP; 
	}else if( (pin & 0b00100000) != 0 ){//PIN8
		PIN8_ENABLE_PULLUP; 
	}else if( (pin & 0b01000000) != 0 ){//PIN9
		PIN9_ENABLE_PULLUP; 
	}else if( (pin & 0b10000000) != 0 ){//PIN10
		PIN10_ENABLE_PULLUP; 
	}
	
}

//Function Arduino-IOTGEMINI to disable Pull Up resistor
void disablePullUp(unsigned char pin){
	if( (pin & 0b00000001) != 0 ){//PIN3
		PIN3_DISABLE_PULLUP; 
	}else if( (pin & 0b00000010) != 0 ){//PIN4
		PIN4_DISABLE_PULLUP; 
	}else if( (pin & 0b00000100) != 0 ){//PIN5
		PIN5_DISABLE_PULLUP; 
	}else if( (pin & 0b00001000) != 0 ){//PIN6
		PIN6_DISABLE_PULLUP; 
	}else if( (pin & 0b00010000) != 0 ){//PIN7
		PIN7_DISABLE_PULLUP; 
	}else if( (pin & 0b00100000) != 0 ){//PIN8
		PIN8_DISABLE_PULLUP; 
	}else if( (pin & 0b01000000) != 0 ){//PIN9
		PIN9_DISABLE_PULLUP; 
	}else if( (pin & 0b10000000) != 0 ){//PIN10
		PIN10_DISABLE_PULLUP; 
	}
	
}


//Function Arduino-IOTGEMINI 
void delayMicroseconds(double us){
	_delay_us(us);	
	
}

//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_BASIC_FUNCTIONS
void 	UART_DEBUG_BASIC_FUNCTIONS_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('B');USART_SendByte('A');USART_SendByte('S');USART_SendByte('I');USART_SendByte('C');USART_SendByte('_');USART_SendByte('F');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}
#endif
