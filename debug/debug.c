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

/*****************************************************************
 * 	DEFINE
*****************************************************************/

/*****************************************************************
 * 	PUBLIC VARIABLES
*****************************************************************/
int cont1_debug;

/*****************************************************************
 * 	PRIVATE VARIABLES
*****************************************************************/

/*****************************************************************
 * 	PUBLIC FUCTIONS
*****************************************************************/
void 	UART_DEBUG_send_STR(unsigned char num_data, unsigned char *data, unsigned char line_feed){
	unsigned char i;
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('M');USART_SendByte('A');USART_SendByte('I');USART_SendByte('N');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}
}

void 	UART_DEBUG_send_STR2(unsigned char *data, unsigned char line_feed){
	unsigned char i;
	unsigned char num_data=strlen(data);
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
		USART_SendByte('(');USART_SendByte('M');USART_SendByte('A');USART_SendByte('I');USART_SendByte('N');USART_SendByte(')');
	}
	for(i=0;i<num_data;i++){
		USART_SendByte((unsigned char)*(data+i));
	}
}

void 	UART_DEBUG_send_NUM_BYTE_HEX(unsigned char byte_to_convert, unsigned char line_feed){
	unsigned char hex_value[2];
	byte_to_hexascii((unsigned char *)&byte_to_convert, hex_value);
	USART_SendByte((unsigned char)'0');
	USART_SendByte((unsigned char)'x');
	USART_SendByte((unsigned char)(hex_value[0]));
	USART_SendByte((unsigned char)(hex_value[1]));
	if(line_feed==1){
		USART_SendByte(0x0D); //carriage return
		USART_SendByte(0x0A); //line feed
	}
}

void 	UART_DEBUG_send_STARS(unsigned char num_star_to_send){
	unsigned char i;
	for(i=0;i<num_star_to_send;i++){
		USART_SendByte('*');
	}
}

/*****************************************************************
 * 	PRIVATE FUCTIONS
*****************************************************************/

