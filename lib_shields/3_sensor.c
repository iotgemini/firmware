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


//***********************************************************************************
//	HERE START THE FUNCTIONS TO BE EDITED TO IMPLEMENT THE FEATURES WANTED
//***********************************************************************************

//Function that allocate the variables and init the shield
void 	init_SENSOR_3(struct settings_shield *currentSettingsShield){
	//the manager init the pin as input thus I have just to add +1 to the variables var_NUM_ANALOGUE_INPUT
	var_NUM_ANALOGUE_INPUT++;
	
	//setting the semaphores
	if((currentSettingsShield->PIN_USED & 0b00000100)!=0){ //ADC0 is used
		root_pin_functions->SEM_ADC |= 0b00000100;
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
		ADMUX = NUM_ADC0;
		//init_average_ADC((unsigned int *)&ADC0_Average_Array[0], (unsigned char *)&contSampleADC0, (unsigned char *)&NUM_MAX_SAMPLE_ADC0, (unsigned char *)&NUM_ADC0);
	}else if((currentSettingsShield->PIN_USED & 0b00010000)!=0){ //ADC4 is used
		root_pin_functions->SEM_ADC |= 0b00010000;
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
		ADMUX = NUM_ADC4;
		//init_average_ADC((unsigned int *)&ADC4_Average_Array[0], (unsigned char *)&contSampleADC4, (unsigned char *)&NUM_MAX_SAMPLE_ADC4, (unsigned char *)&NUM_ADC4);
	}else if((currentSettingsShield->PIN_USED & 0b10000000)!=0){ //ADC5 is used
		root_pin_functions->SEM_ADC |= 0b10000000;
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
		ADMUX = NUM_ADC5;
		//init_average_ADC((unsigned int *)&ADC5_Average_Array[0], (unsigned char *)&contSampleADC5, (unsigned char *)&NUM_MAX_SAMPLE_ADC5, (unsigned char *)&NUM_ADC5);
	}
	
	#ifdef UART_DEBUG_SENSOR_3
	UART_DEBUG_SENSOR_3_send_STR2((unsigned char *)"SEM_ADC=",1); //it send a string through the UART only if is in debug mode
	UART_DEBUG_send_NUM_BYTE_HEX((unsigned char)root_pin_functions->SEM_ADC, 0);
	#endif
}



//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_SENSOR_3
void 	UART_DEBUG_SENSOR_3_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;
	
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('S');USART_SendByte('E');USART_SendByte('N');USART_SendByte('S');USART_SendByte('_');USART_SendByte('3');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}
void 	UART_DEBUG_SENSOR_3_send_STR2(unsigned char *data, unsigned char line_feed){
	unsigned char i;
	unsigned char num_data=strlen(data);

	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('S');USART_SendByte('E');USART_SendByte('N');USART_SendByte('S');USART_SendByte('_');USART_SendByte('3');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}
#endif
