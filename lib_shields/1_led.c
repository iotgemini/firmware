/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			28 / 02 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Functions for Led (also for relay)
	
	
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
void 	init_led_1(void){
	//the manager init the pin as output thus I have just to add +1 to the variables NUM_DIGITAL_OUTPUT
	var_NUM_DIGITAL_OUTPUT++;
}

//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_LED_1
void 	UART_DEBUG_LED_1_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;

	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('L');USART_SendByte('E');USART_SendByte('D');USART_SendByte('_');USART_SendByte('_');USART_SendByte('1');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	
}
#endif
