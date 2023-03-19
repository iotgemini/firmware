/*****************************************************************
	Programmer: 			Emanuele Aimone
	Version:				1.0
	Last Update:			28 / 02 / 2023
	
	
	List compatible MCU: 	ATmega328
	
	Description:			Functions for buttons
	
	
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
void 	init_button_2(struct settings_shield *current_shield_settings){
	//the manager init the pin as input thus I have just to add +1 to the variables NUM_DIGITAL_INPUT
	var_NUM_DIGITAL_INPUT++;
	
	/*if(current_shield_settings->ID == 7){
		//this is the pir sensor that does not needs the pull-up resistor
		PIN4_DISABLE_PULLUP; // pull-up resistor disabled on PD0
	}*/
}



//it send a string through the UART only if is in debug mode
#ifdef UART_DEBUG_BUTTON_2
void 	UART_DEBUG_BUTTON_2_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;

	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('B');USART_SendByte('U');USART_SendByte('T');USART_SendByte('T');USART_SendByte('_');USART_SendByte('2');USART_SendByte(')');
	
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}	

}
#endif
